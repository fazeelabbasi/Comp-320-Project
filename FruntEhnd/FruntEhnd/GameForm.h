#pragma once

#include <thread>
#include <chrono>
#include <string>
#include <math.h>
#include <random>
#include <msclr/marshal.h>
#include <msclr\marshal_cppstd.h>
#include "NetworkClient.h"
#include "User.h"

namespace FrontEnd {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Game
	/// </summary>
	public ref class GameForm : public System::Windows::Forms::Form
	{
	public:
		GameForm(NetworkClient^ networkClient, System::String^ username): networkClient(networkClient), username(username)
		{
			InitializeComponent();
			this->networkClient->onMessage += gcnew Del(this, &GameForm::GameForm_OnMessage);
			this->networkThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &GameForm::receiveLoop));
			this->users = gcnew System::Collections::Generic::List<User^>();
			this->networkThread->Start();
			this->graphPoints = gcnew System::Collections::Generic::List<int>();
			this->refresh();
			this->updateOrCreateUser(this->username, this->money);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~GameForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::String^ username;
	private: double money = 100;
	private: int seed = 1;
	private: int stocks = 0;
	private: delegate void SafeMessageEventDelegate(System::String^ msg);
	private: System::Threading::Thread^ networkThread;
	private: NetworkClient^ networkClient;
	private: msclr::interop::marshal_context context;
	private: System::Collections::Generic::List<int>^ graphPoints;
	private: int graphPointCount = 0;
	private: int graphIndex = 0;
	private: System::Windows::Forms::Button^ btnSell;
	private: System::Collections::Generic::List<User^>^ users;


	private: System::Windows::Forms::TextBox^ txtLogs;
	private: System::Windows::Forms::TextBox^ txtCmd;
	private: System::Windows::Forms::Button^ btnSendPacket;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::Panel^ pnlGraph;
	private: System::Windows::Forms::Button^ btnNewGraph;
	private: System::Windows::Forms::Button^ btnDbgName;
	private: System::Windows::Forms::TextBox^ txtDbgUsername;
	private: System::Windows::Forms::Button^ btnDbgMoney;
	private: System::Windows::Forms::NumericUpDown^ nudDbgMoney;
	private: System::Windows::Forms::Timer^ timGraphTick;
	private: System::Windows::Forms::ListBox^ lstUsers;
	private: System::Windows::Forms::Label^ lblUsername;
	private: System::Windows::Forms::Label^ lblMoney;
	private: System::Windows::Forms::Button^ btnDbgFinish;
	private: System::Windows::Forms::Label^ lblInfo;
	private: System::Windows::Forms::Label^ lblValue;
	private: System::Windows::Forms::Button^ btnBuy;
	private: System::Windows::Forms::Label^ lblStockOwned;
	private: System::Windows::Forms::Button^ btnSellAll;
	private: System::Windows::Forms::Button^ btnBuyAll;
	private: System::ComponentModel::IContainer^ components;

		   /// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->btnSell = (gcnew System::Windows::Forms::Button());
			this->txtLogs = (gcnew System::Windows::Forms::TextBox());
			this->txtCmd = (gcnew System::Windows::Forms::TextBox());
			this->btnSendPacket = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->btnDbgFinish = (gcnew System::Windows::Forms::Button());
			this->btnDbgName = (gcnew System::Windows::Forms::Button());
			this->txtDbgUsername = (gcnew System::Windows::Forms::TextBox());
			this->btnNewGraph = (gcnew System::Windows::Forms::Button());
			this->btnDbgMoney = (gcnew System::Windows::Forms::Button());
			this->nudDbgMoney = (gcnew System::Windows::Forms::NumericUpDown());
			this->pnlGraph = (gcnew System::Windows::Forms::Panel());
			this->lblInfo = (gcnew System::Windows::Forms::Label());
			this->timGraphTick = (gcnew System::Windows::Forms::Timer(this->components));
			this->lstUsers = (gcnew System::Windows::Forms::ListBox());
			this->lblUsername = (gcnew System::Windows::Forms::Label());
			this->lblMoney = (gcnew System::Windows::Forms::Label());
			this->lblValue = (gcnew System::Windows::Forms::Label());
			this->btnBuy = (gcnew System::Windows::Forms::Button());
			this->lblStockOwned = (gcnew System::Windows::Forms::Label());
			this->btnSellAll = (gcnew System::Windows::Forms::Button());
			this->btnBuyAll = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudDbgMoney))->BeginInit();
			this->pnlGraph->SuspendLayout();
			this->SuspendLayout();
			// 
			// btnSell
			// 
			this->btnSell->Location = System::Drawing::Point(413, 373);
			this->btnSell->Name = L"btnSell";
			this->btnSell->Size = System::Drawing::Size(75, 23);
			this->btnSell->TabIndex = 0;
			this->btnSell->Text = L"Sell";
			this->btnSell->UseVisualStyleBackColor = true;
			this->btnSell->Click += gcnew System::EventHandler(this, &GameForm::btnSell_Click);
			// 
			// txtLogs
			// 
			this->txtLogs->Location = System::Drawing::Point(8, 19);
			this->txtLogs->Multiline = true;
			this->txtLogs->Name = L"txtLogs";
			this->txtLogs->ReadOnly = true;
			this->txtLogs->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txtLogs->Size = System::Drawing::Size(245, 71);
			this->txtLogs->TabIndex = 1;
			// 
			// txtCmd
			// 
			this->txtCmd->Location = System::Drawing::Point(9, 97);
			this->txtCmd->Name = L"txtCmd";
			this->txtCmd->Size = System::Drawing::Size(163, 20);
			this->txtCmd->TabIndex = 2;
			this->txtCmd->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &GameForm::txtCmd_KeyPress);
			// 
			// btnSendPacket
			// 
			this->btnSendPacket->Location = System::Drawing::Point(192, 95);
			this->btnSendPacket->Name = L"btnSendPacket";
			this->btnSendPacket->Size = System::Drawing::Size(61, 23);
			this->btnSendPacket->TabIndex = 0;
			this->btnSendPacket->Text = L"Send";
			this->btnSendPacket->UseVisualStyleBackColor = true;
			this->btnSendPacket->Click += gcnew System::EventHandler(this, &GameForm::btnFinish_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->btnDbgFinish);
			this->groupBox1->Controls->Add(this->btnDbgName);
			this->groupBox1->Controls->Add(this->txtDbgUsername);
			this->groupBox1->Controls->Add(this->btnNewGraph);
			this->groupBox1->Controls->Add(this->btnDbgMoney);
			this->groupBox1->Controls->Add(this->nudDbgMoney);
			this->groupBox1->Controls->Add(this->txtLogs);
			this->groupBox1->Controls->Add(this->txtCmd);
			this->groupBox1->Controls->Add(this->btnSendPacket);
			this->groupBox1->Location = System::Drawing::Point(13, 423);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(499, 146);
			this->groupBox1->TabIndex = 3;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Debug";
			// 
			// btnDbgFinish
			// 
			this->btnDbgFinish->Location = System::Drawing::Point(400, 76);
			this->btnDbgFinish->Name = L"btnDbgFinish";
			this->btnDbgFinish->Size = System::Drawing::Size(75, 23);
			this->btnDbgFinish->TabIndex = 9;
			this->btnDbgFinish->Text = L"Finish";
			this->btnDbgFinish->UseVisualStyleBackColor = true;
			this->btnDbgFinish->Click += gcnew System::EventHandler(this, &GameForm::btnDbgFinish_Click);
			// 
			// btnDbgName
			// 
			this->btnDbgName->Location = System::Drawing::Point(400, 47);
			this->btnDbgName->Name = L"btnDbgName";
			this->btnDbgName->Size = System::Drawing::Size(75, 23);
			this->btnDbgName->TabIndex = 6;
			this->btnDbgName->Text = L"Set Name";
			this->btnDbgName->UseVisualStyleBackColor = true;
			this->btnDbgName->Click += gcnew System::EventHandler(this, &GameForm::btnDbgName_Click);
			// 
			// txtDbgUsername
			// 
			this->txtDbgUsername->Location = System::Drawing::Point(274, 47);
			this->txtDbgUsername->Name = L"txtDbgUsername";
			this->txtDbgUsername->Size = System::Drawing::Size(120, 20);
			this->txtDbgUsername->TabIndex = 5;
			// 
			// btnNewGraph
			// 
			this->btnNewGraph->Location = System::Drawing::Point(400, 105);
			this->btnNewGraph->Name = L"btnNewGraph";
			this->btnNewGraph->Size = System::Drawing::Size(75, 23);
			this->btnNewGraph->TabIndex = 5;
			this->btnNewGraph->Text = L"New Data";
			this->btnNewGraph->UseVisualStyleBackColor = true;
			this->btnNewGraph->Click += gcnew System::EventHandler(this, &GameForm::btnNewGraph_Click);
			// 
			// btnDbgMoney
			// 
			this->btnDbgMoney->Location = System::Drawing::Point(400, 17);
			this->btnDbgMoney->Name = L"btnDbgMoney";
			this->btnDbgMoney->Size = System::Drawing::Size(75, 23);
			this->btnDbgMoney->TabIndex = 4;
			this->btnDbgMoney->Text = L"Set Money";
			this->btnDbgMoney->UseVisualStyleBackColor = true;
			this->btnDbgMoney->Click += gcnew System::EventHandler(this, &GameForm::btnDbgMoney_Click);
			// 
			// nudDbgMoney
			// 
			this->nudDbgMoney->Location = System::Drawing::Point(274, 20);
			this->nudDbgMoney->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1215752191, 23, 0, 0 });
			this->nudDbgMoney->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 999999999, 0, 0, System::Int32::MinValue });
			this->nudDbgMoney->Name = L"nudDbgMoney";
			this->nudDbgMoney->Size = System::Drawing::Size(120, 20);
			this->nudDbgMoney->TabIndex = 3;
			// 
			// pnlGraph
			// 
			this->pnlGraph->Controls->Add(this->lblInfo);
			this->pnlGraph->Location = System::Drawing::Point(13, 13);
			this->pnlGraph->Name = L"pnlGraph";
			this->pnlGraph->Size = System::Drawing::Size(655, 354);
			this->pnlGraph->TabIndex = 4;
			this->pnlGraph->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &GameForm::GameForm_OnPaintGraph);
			// 
			// lblInfo
			// 
			this->lblInfo->AutoSize = true;
			this->lblInfo->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 40, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblInfo->Location = System::Drawing::Point(207, 145);
			this->lblInfo->Name = L"lblInfo";
			this->lblInfo->Size = System::Drawing::Size(254, 63);
			this->lblInfo->TabIndex = 0;
			this->lblInfo->Text = L"Waiting...";
			this->lblInfo->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// timGraphTick
			// 
			this->timGraphTick->Enabled = true;
			this->timGraphTick->Interval = 1000;
			this->timGraphTick->Tick += gcnew System::EventHandler(this, &GameForm::timGraphTick_Tick);
			// 
			// lstUsers
			// 
			this->lstUsers->FormattingEnabled = true;
			this->lstUsers->Location = System::Drawing::Point(674, 68);
			this->lstUsers->Name = L"lstUsers";
			this->lstUsers->Size = System::Drawing::Size(153, 134);
			this->lstUsers->TabIndex = 6;
			// 
			// lblUsername
			// 
			this->lblUsername->AutoSize = true;
			this->lblUsername->Location = System::Drawing::Point(675, 25);
			this->lblUsername->Name = L"lblUsername";
			this->lblUsername->Size = System::Drawing::Size(67, 13);
			this->lblUsername->TabIndex = 7;
			this->lblUsername->Text = L"<Username>";
			// 
			// lblMoney
			// 
			this->lblMoney->AutoSize = true;
			this->lblMoney->Location = System::Drawing::Point(674, 42);
			this->lblMoney->Name = L"lblMoney";
			this->lblMoney->Size = System::Drawing::Size(51, 13);
			this->lblMoney->TabIndex = 8;
			this->lblMoney->Text = L"<Money>";
			// 
			// lblValue
			// 
			this->lblValue->AutoSize = true;
			this->lblValue->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblValue->Location = System::Drawing::Point(282, 379);
			this->lblValue->Name = L"lblValue";
			this->lblValue->Size = System::Drawing::Size(111, 29);
			this->lblValue->TabIndex = 0;
			this->lblValue->Text = L"Waiting...";
			this->lblValue->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// btnBuy
			// 
			this->btnBuy->Location = System::Drawing::Point(175, 373);
			this->btnBuy->Name = L"btnBuy";
			this->btnBuy->Size = System::Drawing::Size(75, 23);
			this->btnBuy->TabIndex = 9;
			this->btnBuy->Text = L"Buy";
			this->btnBuy->UseVisualStyleBackColor = true;
			this->btnBuy->Click += gcnew System::EventHandler(this, &GameForm::btnBuy_Click);
			// 
			// lblStockOwned
			// 
			this->lblStockOwned->AutoSize = true;
			this->lblStockOwned->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblStockOwned->Location = System::Drawing::Point(538, 373);
			this->lblStockOwned->Name = L"lblStockOwned";
			this->lblStockOwned->Size = System::Drawing::Size(111, 29);
			this->lblStockOwned->TabIndex = 0;
			this->lblStockOwned->Text = L"Waiting...";
			this->lblStockOwned->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// btnSellAll
			// 
			this->btnSellAll->Location = System::Drawing::Point(413, 402);
			this->btnSellAll->Name = L"btnSellAll";
			this->btnSellAll->Size = System::Drawing::Size(75, 23);
			this->btnSellAll->TabIndex = 10;
			this->btnSellAll->Text = L"Sell All";
			this->btnSellAll->UseVisualStyleBackColor = true;
			this->btnSellAll->Click += gcnew System::EventHandler(this, &GameForm::btnSellAll_Click);
			// 
			// btnBuyAll
			// 
			this->btnBuyAll->Location = System::Drawing::Point(175, 402);
			this->btnBuyAll->Name = L"btnBuyAll";
			this->btnBuyAll->Size = System::Drawing::Size(75, 23);
			this->btnBuyAll->TabIndex = 10;
			this->btnBuyAll->Text = L"Buy All";
			this->btnBuyAll->UseVisualStyleBackColor = true;
			this->btnBuyAll->Click += gcnew System::EventHandler(this, &GameForm::btnBuyAll_Click);
			// 
			// GameForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(839, 581);
			this->Controls->Add(this->btnBuyAll);
			this->Controls->Add(this->btnSellAll);
			this->Controls->Add(this->btnBuy);
			this->Controls->Add(this->lblStockOwned);
			this->Controls->Add(this->lblValue);
			this->Controls->Add(this->lblMoney);
			this->Controls->Add(this->lblUsername);
			this->Controls->Add(this->lstUsers);
			this->Controls->Add(this->pnlGraph);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->btnSell);
			this->Name = L"GameForm";
			this->Text = L"Game";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &GameForm::GameForm_FormClosed);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudDbgMoney))->EndInit();
			this->pnlGraph->ResumeLayout(false);
			this->pnlGraph->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
/*
====================================================
====	App Functionality
====================================================
*/
	private: System::Void GameForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e) {
		this->networkClient->stop();
		Application::Exit();
	}

	private: void receiveLoop() {
		networkClient->receiveInfo();
	}

	private: System::Void log(std::string msg) {
		this->log(gcnew System::String(msg.c_str()));
	}

	private: System::Void log(System::String^ msg) {
		this->txtLogs->AppendText(msg);
		this->txtLogs->AppendText("\n");
	}

/*
====================================================
====	Debug Logic
====================================================
*/
	private: void executeCommand() {
		this->log(System::String::Format("Sent <{0}>", this->txtCmd->Text));
		networkClient->sendInfo(this->txtCmd->Text);
		this->txtCmd->Text = "";
	}

	private: System::Void btnNewGraph_Click(System::Object^ sender, System::EventArgs^ e) {
		System::Random^ r = gcnew System::Random();
		this->generateGraphValues(this->seed = r->Next());
	}
	private: System::Void btnDbgMoney_Click(System::Object^ sender, System::EventArgs^ e) {
		this->money = System::Decimal::ToDouble(this->nudDbgMoney->Value);
		this->log(System::String::Format("Set money to {0:C}", this->money));
		this->refresh();
	}
	private: System::Void btnDbgName_Click(System::Object^ sender, System::EventArgs^ e) {
		this->username = this->txtDbgUsername->Text;
		this->log(System::String::Format("Set username to <{0}>", this->username));
		this->refresh();
	}
	private: System::Void txtCmd_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ kp) {
		//this->log(System::String::Format("pressed <{0}>, checking <{1}>", kp->KeyChar, Keys::Enter));
		if (kp->KeyChar == 13) {
			executeCommand();
		}
	}
	private: System::Void btnDbgFinish_Click(System::Object^ sender, System::EventArgs^ e) {
		this->networkClient->sendInfo(System::String::Format("FINISH {0}", this->username));
	}
		   
	private: System::Void btnFinish_Click(System::Object^ sender, System::EventArgs^ e) {
		executeCommand();
	}
/*
====================================================
====	Game Logic
====================================================
*/
	private: void startRound(int seed) {
		this->generateGraphValues(seed);
		this->lblInfo->Text = "";
	}

	private: void roundTick() {
		this->graphIndex++;
		switch (this->graphIndex - this->graphPointCount) {
		case 3: {
			this->lblInfo->Text = "Crash!";
			break;
		}
		case 5:
			this->stocks = 0;
			this->networkClient->sendInfo(System::String::Format("FINISH {0}", this->username));
			this->networkClient->sendInfo(System::String::Format("BALANCE {0} {1}", this->username, this->money));
			break;
		}
		this->refresh();
	}

	private: int getStockValue() {
		if (this->graphPointCount == 0)
			return -1;
		if (this->graphIndex >= this->graphPointCount)
			return -1;
		return this->graphPoints[this->graphIndex];
	}

	private: System::Void timGraphTick_Tick(System::Object^ sender, System::EventArgs^ e) {
		this->roundTick();
	}

	private: System::Void btnSell_Click(System::Object^ sender, System::EventArgs^ e) {
		this->stocks -= 1;
		this->money += this->getStockValue();
		this->refresh();
	}

	private: System::Void btnBuy_Click(System::Object^ sender, System::EventArgs^ e) {
		this->money -= this->getStockValue();
		this->stocks++;
		this->refresh();
	}

	private: System::Void btnSellAll_Click(System::Object^ sender, System::EventArgs^ e) {
		this->money += this->getStockValue() * this->stocks;
		this->stocks = 0; 
		this->refresh();
	}
	private: System::Void btnBuyAll_Click(System::Object^ sender, System::EventArgs^ e) {
		while (this->money > this->getStockValue()) {
			this->money -= this->getStockValue();
			this->stocks++;
		}
		this->refresh();
	}

/*
====================================================
====	Networking, Message Handler
====================================================
*/
	private: System::Void GameForm_OnMessage(System::String^ msg) {
		try {
			if (this->IsDisposed)
				return;
			if (!this->Created)
				return;
			SafeMessageEventDelegate^ d = gcnew SafeMessageEventDelegate(this, &GameForm::GameForm_OnMessageSafe);

			for each (auto line in msg->Split(gcnew array<System::String^>{ gcnew System::String("\n") }, System::StringSplitOptions::RemoveEmptyEntries))
				this->Invoke(d, line);
		}
		catch (Exception^ ignored) {}
	}

	private: System::Void GameForm_OnMessageSafe(System::String^ msg) {
		this->log(System::String::Format("Received <{0}>", msg));
		auto args = msg->Split();
		if (msg->Length > 5 && args->Length == 3 && msg->Substring(0, 5) == "USER ") {
			this->updateOrCreateUser(args[1], System::Convert::ToDouble(args[2]));
		}
		if (msg->Length > 5 && args->Length == 2 && msg->Substring(0, 5) == "KICK ") {
			this->kick(args[1]);
		}
		if (msg->Length >= 5 && args->Length == 2 && msg->Substring(0, 5) == "SEED ") {
			this->startRound(System::Convert::ToInt16(args[1]));
		}
		this->refresh();
	}

/*
====================================================
====	Utils
====================================================
*/
	private: System::Void updateOrCreateUser(System::String^ name, double money) {
		// update if exists
		for each (auto u in users) {
			if (u->username == name) {
				u->money = money;
				this->log(System::String::Format("Set <{0}> money to {1:C}", name, money));
				return;
			}
		}
		this->log(System::String::Format("Created user <{0}> with money {1:C}", name, money));
		User^ u = gcnew User(name, money);
		this->users->Add(u);
		if (name == this->username)
			this->money = money;
		this->refresh();
	}


	private: User^ getUser(System::String^ name) {
		for each (auto u in users)
			if (u->username == name)
				return u;
		return nullptr;
	}

	private: System::Boolean userExists(System::String^ name) {
		return this->getUser(name) == nullptr;
	}

	private: System::Void kick(System::String^ name) {
		User^ u;
		if (u = getUser(name)) {
			this->users->Remove(u);
			this->log(System::String::Format("Kicked <{0}>", name));
			return;
		}
		this->log(System::String::Format("Failed to kick <{0}>, doesn't exist", name));
	}



	private: System::Void generateGraphValues(int randSeed) {
		System::Random r(randSeed);
		this->graphPoints->Clear();
		double crashChance = 0.005;
		int x = 0;
		this->graphPoints->Add(1);
		this->graphPointCount = 0;
		this->graphIndex = 3;
		for (int i = 0; i < 25 && r.NextDouble() > (crashChance*=1.1) ; i++) {
			x = x + (int)(r.NextDouble() * 200) - 100;
			x = x < 0 ? 0 : x;
			this->graphPoints->Add(x);
			this->graphPointCount++;
		}
		this->graphPoints->Add(0);
		this->graphPointCount++;
		this->pnlGraph->Refresh();
	}

/*
====================================================
====	Rendering
====================================================
*/
	private: void refresh() {
		this->lblUsername->Text = this->username;
		this->lblMoney->Text = System::String::Format("{0:C}", this->money);
		this->pnlGraph->Refresh();
		if (this->getStockValue() >= 0) {
			this->lblValue->Text = System::String::Format("{0:C}", this->getStockValue());
			this->lblStockOwned->Text = System::String::Format("Owned: {0}\nValue: {0:C}", this->stocks, this->stocks * this->getStockValue());
			this->btnSell->Enabled = this->stocks > 0;
			this->btnSellAll->Enabled = this->btnSell->Enabled;
			this->btnBuy->Enabled = this->money > this->getStockValue() && this->getStockValue() > 0;
			this->btnBuyAll->Enabled = this->btnBuy->Enabled;
		}
		else {
			this->btnBuy->Enabled = false;
			this->btnSell->Enabled = false;
		}
		this->lstUsers->Items->Clear();
		for each (auto u in this->users) {
			if (u->username == this->username)
				continue;
			this->lstUsers->Items->Add(System::String::Format("<{0}> {1:C}", u->username, u->money));
		}
		this->Refresh();
	}

	private: System::Void GameForm_OnPaintGraph(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ pe) {
		Graphics^ g = pe->Graphics;
		g->Clear(Color::AntiqueWhite);
		if (this->graphPointCount == 0)
			return;
		System::Drawing::Rectangle area = this->pnlGraph->ClientRectangle;
		const int padding = 10;
		Pen^ redPen = gcnew Pen(Color::Red);
		redPen->Width = 4;
		Pen^ blackPen = gcnew Pen(Color::Black);
		blackPen->Width = 1;

		double min = 0, max = 0, step = 0, scale=1;
		int i = 0;
		for each (int d in this->graphPoints) {
			i++;
			if (i >= this->graphIndex)
				break;
			if (d < min)
				min = d;
			if (d > max)
				max = d;
			step++;
		}
		step = (area.Width - 20)/step;
		scale = (area.Height - 20)/max;
		g->DrawLine(blackPen, 0, 0, area.Width, area.Height);
		g->DrawLine(blackPen, 0, area.Height+padding, area.Width+padding, area.Height);
		int prevX = padding/2, prevY = area.Height -this->graphPoints[0] * scale - padding/2;
		for (i = 0; i < this->graphIndex && i<this->graphPointCount; i++) {
			int d = this->graphPoints[i];
			int scaledY = area.Height - 2 - (d * scale);
			g->DrawLine(redPen, prevX-padding, prevY - padding, prevX + step - padding, scaledY - padding);
			prevX += step;
			prevY = scaledY;
		}
	}

};
}
