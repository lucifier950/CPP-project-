#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set up the central widget and stacked widget for page switching
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackedWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Add default admin account
    userCredentials["admin"] = "password";

    // Setup all pages
    setupLoginPage();
    setupSignUpPage();
    setupDashboardPage();
    setupMembersPage();
    setupWorkoutPage();
    setupCaloriesPage();

    // Add pages to stacked widget
    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(signUpPage);
    stackedWidget->addWidget(dashboardPage);
    stackedWidget->addWidget(membersPage);
    stackedWidget->addWidget(workoutPage);
    stackedWidget->addWidget(caloriesPage);

    // Start with login page
    stackedWidget->setCurrentWidget(loginPage);

    // Load sample data
    loadSampleData();
}

MainWindow::~MainWindow()
{
    // Qt's parent-child relationship will handle memory cleanup
}

void MainWindow::loadSampleData()
{
    // Sample members
    Member member1;
    member1.name = "John Smith";
    member1.phone = "555-1234";
    member1.email = "john@example.com";
    member1.membershipType = "Annual";
    member1.joinDate = QDate::currentDate().addMonths(-3);
    member1.expiryDate = member1.joinDate.addYears(1);
    members.append(member1);

    Member member2;
    member2.name = "Sarah Johnson";
    member2.phone = "555-5678";
    member2.email = "sarah@example.com";
    member2.membershipType = "Monthly";
    member2.joinDate = QDate::currentDate().addMonths(-1);
    member2.expiryDate = member2.joinDate.addMonths(1);
    members.append(member2);

    // Sample workouts
    Workout workout1;
    workout1.name = "Bench Press";
    workout1.target = "Chest";
    workout1.description = "Lie on bench, lower barbell to chest, press up";
    workout1.sets = 3;
    workout1.reps = 10;
    workouts.append(workout1);

    Workout workout2;
    workout2.name = "Squat";
    workout2.target = "Legs";
    workout2.description = "Stand with barbell on shoulders, bend knees, return to standing";
    workout2.sets = 4;
    workout2.reps = 8;
    workouts.append(workout2);

    Workout workout3;
    workout3.name = "Pull-ups";
    workout3.target = "Back";
    workout3.description = "Hang from bar, pull body up until chin over bar";
    workout3.sets = 3;
    workout3.reps = 8;
    workouts.append(workout3);
}

void MainWindow::setupLoginPage()
{
    // Create login page widget
    loginPage = new QWidget(this);
    loginLayout = new QGridLayout(loginPage);

    // Create title
    loginTitleLabel = new QLabel("FitLife Gym", this);
    QFont titleFont = loginTitleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    loginTitleLabel->setFont(titleFont);
    loginTitleLabel->setAlignment(Qt::AlignCenter);

    // Create username components
    usernameLabel = new QLabel("Username:", this);
    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Enter your username");

    // Create password components
    passwordLabel = new QLabel("Password:", this);
    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Enter your password");
    passwordInput->setEchoMode(QLineEdit::Password);

    // Create buttons
    loginButton = new QPushButton("Login", this);
    clearLoginButton = new QPushButton("Clear", this);
    showSignUpButton = new QPushButton("Create New Account", this);

    // Create status label
    loginStatusLabel = new QLabel("Please log in to access the gym management system", this);
    loginStatusLabel->setAlignment(Qt::AlignCenter);

    // Add widgets to layout
    loginLayout->addWidget(loginTitleLabel, 0, 0, 1, 2);
    loginLayout->addWidget(new QLabel("Login to Your Account", this), 1, 0, 1, 2, Qt::AlignCenter);
    loginLayout->addWidget(usernameLabel, 2, 0);
    loginLayout->addWidget(usernameInput, 2, 1);
    loginLayout->addWidget(passwordLabel, 3, 0);
    loginLayout->addWidget(passwordInput, 3, 1);

    // Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(clearLoginButton);

    loginLayout->addLayout(buttonLayout, 4, 0, 1, 2);
    loginLayout->addWidget(showSignUpButton, 5, 0, 1, 2);
    loginLayout->addWidget(loginStatusLabel, 6, 0, 1, 2);

    // Set some spacing and margins
    loginLayout->setSpacing(15);
    loginLayout->setContentsMargins(40, 40, 40, 40);

    // Add vertical space
    loginLayout->setRowStretch(7, 1);

    // Connect signals and slots
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginButtonClicked);
    connect(clearLoginButton, &QPushButton::clicked, this, &MainWindow::onClearLoginButtonClicked);
    connect(showSignUpButton, &QPushButton::clicked, this, &MainWindow::onShowSignUpClicked);
}

void MainWindow::setupSignUpPage()
{
    // Create sign up page widget
    signUpPage = new QWidget(this);
    signUpLayout = new QGridLayout(signUpPage);

    // Create title
    signUpTitleLabel = new QLabel("FitLife Gym", this);
    QFont titleFont = signUpTitleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    signUpTitleLabel->setFont(titleFont);
    signUpTitleLabel->setAlignment(Qt::AlignCenter);

    // Create username components
    newUsernameLabel = new QLabel("Username:", this);
    newUsernameInput = new QLineEdit(this);
    newUsernameInput->setPlaceholderText("Choose a username");

    // Create password components
    newPasswordLabel = new QLabel("Password:", this);
    newPasswordInput = new QLineEdit(this);
    newPasswordInput->setPlaceholderText("Create a password");
    newPasswordInput->setEchoMode(QLineEdit::Password);

    // Create confirm password components
    confirmPasswordLabel = new QLabel("Confirm Password:", this);
    confirmPasswordInput = new QLineEdit(this);
    confirmPasswordInput->setPlaceholderText("Confirm your password");
    confirmPasswordInput->setEchoMode(QLineEdit::Password);

    // Create email components
    emailLabel = new QLabel("Email:", this);
    emailInput = new QLineEdit(this);
    emailInput->setPlaceholderText("Enter your email");

    // Create buttons
    signUpButton = new QPushButton("Register", this);
    clearSignUpButton = new QPushButton("Clear", this);
    showLoginButton = new QPushButton("Back to Login", this);

    // Create status label
    signUpStatusLabel = new QLabel("Please fill in all fields to create your account", this);
    signUpStatusLabel->setAlignment(Qt::AlignCenter);

    // Add widgets to layout
    signUpLayout->addWidget(signUpTitleLabel, 0, 0, 1, 2);
    signUpLayout->addWidget(new QLabel("Create New Account", this), 1, 0, 1, 2, Qt::AlignCenter);
    signUpLayout->addWidget(newUsernameLabel, 2, 0);
    signUpLayout->addWidget(newUsernameInput, 2, 1);
    signUpLayout->addWidget(newPasswordLabel, 3, 0);
    signUpLayout->addWidget(newPasswordInput, 3, 1);
    signUpLayout->addWidget(confirmPasswordLabel, 4, 0);
    signUpLayout->addWidget(confirmPasswordInput, 4, 1);
    signUpLayout->addWidget(emailLabel, 5, 0);
    signUpLayout->addWidget(emailInput, 5, 1);

    // Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(signUpButton);
    buttonLayout->addWidget(clearSignUpButton);

    signUpLayout->addLayout(buttonLayout, 6, 0, 1, 2);
    signUpLayout->addWidget(showLoginButton, 7, 0, 1, 2);
    signUpLayout->addWidget(signUpStatusLabel, 8, 0, 1, 2);

    // Set some spacing and margins
    signUpLayout->setSpacing(15);
    signUpLayout->setContentsMargins(40, 40, 40, 40);

    // Add vertical space
    signUpLayout->setRowStretch(9, 1);

    // Connect signals and slots
    connect(signUpButton, &QPushButton::clicked, this, &MainWindow::onSignUpButtonClicked);
    connect(clearSignUpButton, &QPushButton::clicked, this, &MainWindow::onClearSignUpButtonClicked);
    connect(showLoginButton, &QPushButton::clicked, this, &MainWindow::onShowLoginClicked);
}

void MainWindow::setupDashboardPage()
{
    // Create dashboard page widget
    dashboardPage = new QWidget(this);
    dashboardLayout = new QVBoxLayout(dashboardPage);

    // Create welcome label
    welcomeLabel = new QLabel("Welcome to FitLife Gym Management System", this);
    QFont titleFont = welcomeLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    welcomeLabel->setFont(titleFont);
    welcomeLabel->setAlignment(Qt::AlignCenter);

    // Create dashboard buttons
    membersButton = new QPushButton("Manage Members", this);
    membersButton->setMinimumHeight(60);

    workoutPlanButton = new QPushButton("Workout Plans", this);
    workoutPlanButton->setMinimumHeight(60);

    calorieCalculatorButton = new QPushButton("Calorie Calculator", this);
    calorieCalculatorButton->setMinimumHeight(60);

    logoutButton = new QPushButton("Logout", this);

    // Add widgets to layout
    dashboardLayout->addWidget(welcomeLabel);
    dashboardLayout->addSpacing(30);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(membersButton);
    buttonsLayout->addWidget(workoutPlanButton);
    buttonsLayout->addWidget(calorieCalculatorButton);

    dashboardLayout->addLayout(buttonsLayout);
    dashboardLayout->addSpacing(30);
    dashboardLayout->addWidget(logoutButton, 0, Qt::AlignRight);

    // Add stretch to push everything to the top
    dashboardLayout->addStretch();

    // Set some spacing and margins
    dashboardLayout->setSpacing(15);
    dashboardLayout->setContentsMargins(40, 40, 40, 40);

    // Connect signals and slots
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::onLogoutButtonClicked);
    connect(membersButton, &QPushButton::clicked, this, &MainWindow::onMembersButtonClicked);
    connect(workoutPlanButton, &QPushButton::clicked, this, &MainWindow::onWorkoutPlanButtonClicked);
    connect(calorieCalculatorButton, &QPushButton::clicked, this, &MainWindow::onCalorieCalculatorButtonClicked);
}

void MainWindow::setupMembersPage()
{
    // Create members page widget
    membersPage = new QWidget(this);
    membersLayout = new QVBoxLayout(membersPage);

    // Create title
    membersTitle = new QLabel("Members Management", this);
    QFont titleFont = membersTitle->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    membersTitle->setFont(titleFont);
    membersTitle->setAlignment(Qt::AlignCenter);

    // Create table
    membersTable = new QTableWidget(0, 6, this);
    QStringList headers;
    headers << "Name" << "Phone" << "Email" << "Membership Type" << "Join Date" << "Expiry Date";
    membersTable->setHorizontalHeaderLabels(headers);
    membersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    membersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    membersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Create input fields for adding/editing members
    QGridLayout *inputLayout = new QGridLayout();
    inputLayout->addWidget(new QLabel("Name:"), 0, 0);
    memberNameInput = new QLineEdit(this);
    inputLayout->addWidget(memberNameInput, 0, 1);

    inputLayout->addWidget(new QLabel("Phone:"), 0, 2);
    memberPhoneInput = new QLineEdit(this);
    inputLayout->addWidget(memberPhoneInput, 0, 3);

    inputLayout->addWidget(new QLabel("Email:"), 1, 0);
    memberEmailInput = new QLineEdit(this);
    inputLayout->addWidget(memberEmailInput, 1, 1);

    inputLayout->addWidget(new QLabel("Membership Type:"), 1, 2);
    membershipTypeCombo = new QComboBox(this);
    membershipTypeCombo->addItems(QStringList() << "Monthly" << "Quarterly" << "Annual");
    inputLayout->addWidget(membershipTypeCombo, 1, 3);

    // Create buttons
    memberButtonsLayout = new QHBoxLayout();
    addMemberButton = new QPushButton("Add Member", this);
    editMemberButton = new QPushButton("Update Selected", this);
    deleteMemberButton = new QPushButton("Delete Selected", this);
    backToMainButton = new QPushButton("Back to Main Menu", this);

    memberButtonsLayout->addWidget(addMemberButton);
    memberButtonsLayout->addWidget(editMemberButton);
    memberButtonsLayout->addWidget(deleteMemberButton);
    memberButtonsLayout->addWidget(backToMainButton);

    // Add widgets to layout
    membersLayout->addWidget(membersTitle);
    membersLayout->addWidget(membersTable);
    membersLayout->addLayout(inputLayout);
    membersLayout->addLayout(memberButtonsLayout);

    // Set some spacing and margins
    membersLayout->setSpacing(15);
    membersLayout->setContentsMargins(20, 20, 20, 20);

    // Connect signals and slots
    connect(addMemberButton, &QPushButton::clicked, this, &MainWindow::onAddMemberButtonClicked);
    connect(deleteMemberButton, &QPushButton::clicked, this, &MainWindow::onDeleteMemberButtonClicked);
    connect(editMemberButton, &QPushButton::clicked, this, &MainWindow::onEditMemberButtonClicked);
    connect(backToMainButton, &QPushButton::clicked, this, &MainWindow::onBackToMainButtonClicked);
}

void MainWindow::setupWorkoutPage()
{
    // Create workout page widget
    workoutPage = new QWidget(this);
    workoutLayout = new QVBoxLayout(workoutPage);

    // Create title
    workoutTitle = new QLabel("Workout Plans", this);
    QFont titleFont = workoutTitle->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    workoutTitle->setFont(titleFont);
    workoutTitle->setAlignment(Qt::AlignCenter);

    // Create table
    workoutTable = new QTableWidget(0, 5, this);
    QStringList headers;
    headers << "Exercise Name" << "Target Muscle" << "Description" << "Sets" << "Reps";
    workoutTable->setHorizontalHeaderLabels(headers);
    workoutTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    workoutTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    workoutTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Create input fields for adding/editing workouts
    QGridLayout *inputLayout = new QGridLayout();
    inputLayout->addWidget(new QLabel("Exercise Name:"), 0, 0);
    workoutNameInput = new QLineEdit(this);
    inputLayout->addWidget(workoutNameInput, 0, 1);

    inputLayout->addWidget(new QLabel("Target Muscle:"), 0, 2);
    workoutTargetInput = new QLineEdit(this);
    inputLayout->addWidget(workoutTargetInput, 0, 3);

    inputLayout->addWidget(new QLabel("Description:"), 1, 0);
    workoutDescInput = new QTextEdit(this);
    workoutDescInput->setMaximumHeight(60);
    inputLayout->addWidget(workoutDescInput, 1, 1, 1, 3);

    inputLayout->addWidget(new QLabel("Sets:"), 2, 0);
    workoutSetsInput = new QSpinBox(this);
    workoutSetsInput->setMinimum(1);
    workoutSetsInput->setMaximum(10);
    inputLayout->addWidget(workoutSetsInput, 2, 1);

    inputLayout->addWidget(new QLabel("Reps:"), 2, 2);
    workoutRepsInput = new QSpinBox(this);
    workoutRepsInput->setMinimum(1);
    workoutRepsInput->setMaximum(30);
    inputLayout->addWidget(workoutRepsInput, 2, 3);

    // Create buttons
    workoutButtonsLayout = new QHBoxLayout();
    addWorkoutButton = new QPushButton("Add Workout", this);
    deleteWorkoutButton = new QPushButton("Delete Selected", this);
    backFromWorkoutButton = new QPushButton("Back to Main Menu", this);

    workoutButtonsLayout->addWidget(addWorkoutButton);
    workoutButtonsLayout->addWidget(deleteWorkoutButton);
    workoutButtonsLayout->addWidget(backFromWorkoutButton);

    // Add widgets to layout
    workoutLayout->addWidget(workoutTitle);
    workoutLayout->addWidget(workoutTable);
    workoutLayout->addLayout(inputLayout);
    workoutLayout->addLayout(workoutButtonsLayout);

    // Set some spacing and margins
    workoutLayout->setSpacing(15);
    workoutLayout->setContentsMargins(20, 20, 20, 20);

    // Connect signals and slots
    connect(addWorkoutButton, &QPushButton::clicked, this, &MainWindow::onAddWorkoutButtonClicked);
    connect(deleteWorkoutButton, &QPushButton::clicked, this, &MainWindow::onDeleteWorkoutButtonClicked);
    connect(backFromWorkoutButton, &QPushButton::clicked, this, &MainWindow::onBackFromWorkoutButtonClicked);
}

void MainWindow::setupCaloriesPage()
{
    // Create calories page widget
    caloriesPage = new QWidget(this);
    caloriesLayout = new QVBoxLayout(caloriesPage);

    // Create title
    caloriesTitle = new QLabel("Calorie Calculator", this);
    QFont titleFont = caloriesTitle->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    caloriesTitle->setFont(titleFont);
    caloriesTitle->setAlignment(Qt::AlignCenter);

    // Personal Information Group
    personalInfoGroup = new QGroupBox("Personal Information");
    personalInfoLayout = new QFormLayout();

    ageInput = new QSpinBox(this);
    ageInput->setRange(13, 100);
    ageInput->setValue(30);
    personalInfoLayout->addRow("Age:", ageInput);

    genderGroup = new QButtonGroup(this);
    maleRadio = new QRadioButton("Male", this);
    femaleRadio = new QRadioButton("Female", this);
    maleRadio->setChecked(true);
    genderGroup->addButton(maleRadio);
    genderGroup->addButton(femaleRadio);

    QHBoxLayout *genderLayout = new QHBoxLayout();
    genderLayout->addWidget(maleRadio);
    genderLayout->addWidget(femaleRadio);
    personalInfoLayout->addRow("Gender:", genderLayout);

    weightInput = new QDoubleSpinBox(this);
    weightInput->setRange(30.0, 300.0);
    weightInput->setValue(70.0);
    weightInput->setSuffix(" kg");
    personalInfoLayout->addRow("Weight:", weightInput);

    heightInput = new QDoubleSpinBox(this);
    heightInput->setRange(100.0, 250.0);
    heightInput->setValue(170.0);
    heightInput->setSuffix(" cm");
    personalInfoLayout->addRow("Height:", heightInput);

    personalInfoGroup->setLayout(personalInfoLayout);

    // Activity Level Group
    activityGroup = new QGroupBox("Activity Level");
    activityLayout = new QFormLayout();

    activityLevelCombo = new QComboBox(this);
    activityLevelCombo->addItem("Sedentary (little or no exercise)");
    activityLevelCombo->addItem("Lightly active (light exercise 1-3 days/week)");
    activityLevelCombo->addItem("Moderately active (moderate exercise 3-5 days/week)");
    activityLevelCombo->addItem("Very active (hard exercise 6-7 days/week)");
    activityLevelCombo->addItem("Extra active (very hard exercise & physical job)");

    activityLayout->addRow("Activity Level:", activityLevelCombo);

    activityDescriptionLabel = new QLabel("Your activity level affects your daily calorie needs", this);
    activityDescriptionLabel->setWordWrap(true);
    activityLayout->addRow(activityDescriptionLabel);

    activityGroup->setLayout(activityLayout);

    // Results Group
    resultsGroup = new QGroupBox("Calorie Results");
    resultsLayout = new QFormLayout();

    bmrResultLabel = new QLabel("0 calories/day", this);
    tdeeResultLabel = new QLabel("0 calories/day", this);
    weightLossLabel = new QLabel("0 calories/day", this);
    maintainWeightLabel = new QLabel("0 calories/day", this);
    weightGainLabel = new QLabel("0 calories/day", this);

    resultsLayout->addRow("Basal Metabolic Rate (BMR):", bmrResultLabel);
    resultsLayout->addRow("Total Daily Energy Expenditure:", tdeeResultLabel);
    resultsLayout->addRow("Weight Loss (0.5kg/week):", weightLossLabel);
    resultsLayout->addRow("Maintain Weight:", maintainWeightLabel);
    resultsLayout->addRow("Weight Gain (0.5kg/week):", weightGainLabel);

    resultsGroup->setLayout(resultsLayout);

    // Buttons
    calculateCaloriesButton = new QPushButton("Calculate", this);
    backFromCaloriesButton = new QPushButton("Back to Main Menu", this);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(calculateCaloriesButton);
    buttonsLayout->addWidget(backFromCaloriesButton);

    // Add widgets to layout
    caloriesLayout->addWidget(caloriesTitle);
    caloriesLayout->addWidget(personalInfoGroup);
    caloriesLayout->addWidget(activityGroup);
    caloriesLayout->addWidget(resultsGroup);
    caloriesLayout->addLayout(buttonsLayout);

    // Set some spacing and margins
    caloriesLayout->setSpacing(15);
    caloriesLayout->setContentsMargins(20, 20, 20, 20);

    // Connect signals and slots
    connect(calculateCaloriesButton, &QPushButton::clicked, this, &MainWindow::onCalculateCaloriesButtonClicked);
    connect(backFromCaloriesButton, &QPushButton::clicked, this, &MainWindow::onBackFromCaloriesButtonClicked);
    connect(activityLevelCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onActivityLevelChanged);
    connect(maleRadio, &QRadioButton::toggled, this, &MainWindow::onGenderSelectionChanged);
}

void MainWindow::onLoginButtonClicked()
{
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    // Check if username exists and password matches
    if (userCredentials.contains(username) && userCredentials[username] == password) {
        currentUser = username;
        loginStatusLabel->setText("Login successful!");
        welcomeLabel->setText("Welcome to FitLife Gym Management System, " + username + "!");

        // Update tables
        updateMembersTable();
        updateWorkoutTable();

        // Switch to dashboard
        stackedWidget->setCurrentWidget(dashboardPage);
    } else {
        loginStatusLabel->setText("Login failed. Please try again.");
        QMessageBox::warning(this, "Login Failed", "Invalid username or password!");
    }
}

void MainWindow::onClearLoginButtonClicked()
{
    usernameInput->clear();
    passwordInput->clear();
    loginStatusLabel->setText("Please log in to access the gym management system");
    usernameInput->setFocus();
}

void MainWindow::onShowSignUpClicked()
{
    // Switch to sign up page
    stackedWidget->setCurrentWidget(signUpPage);

    // Clear any previous entries
    onClearSignUpButtonClicked();
}

void MainWindow::onSignUpButtonClicked()
{
    QString username = newUsernameInput->text();
    QString password = newPasswordInput->text();
    QString confirmPassword = confirmPasswordInput->text();
    QString email = emailInput->text();

    // Validate inputs
    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty() || email.isEmpty()) {
        signUpStatusLabel->setText("Please fill in all fields");
        QMessageBox::warning(this, "Registration Failed", "All fields are required!");
        return;
    }

    if (password != confirmPassword) {
        signUpStatusLabel->setText("Passwords do not match");
        QMessageBox::warning(this, "Registration Failed", "Passwords do not match!");
        return;
    }

    if (userCredentials.contains(username)) {
        signUpStatusLabel->setText("Username already exists");
        QMessageBox::warning(this, "Registration Failed", "Username already exists. Please choose another one.");
        return;
    }

    // Simple email validation (basic format check)
    if (!email.contains("@") || !email.contains(".")) {
        signUpStatusLabel->setText("Invalid email format");
        QMessageBox::warning(this, "Registration Failed", "Invalid email format!");
        return;
    }

    // Register the new user
    userCredentials[username] = password;

    signUpStatusLabel->setText("Registration successful!");
    QMessageBox::information(this, "Registration Successful",
                             "Your account has been created successfully!\nYou can now log in.");

    // Switch back to login page
    stackedWidget->setCurrentWidget(loginPage);

    // Pre-fill the username
    usernameInput->setText(username);
    passwordInput->clear();
    loginStatusLabel->setText("Please enter your password");
    passwordInput->setFocus();
}

void MainWindow::onClearSignUpButtonClicked()
{
    newUsernameInput->clear();
    newPasswordInput->clear();
    confirmPasswordInput->clear();
    emailInput->clear();
    signUpStatusLabel->setText("Please fill in all fields to create your account");
    newUsernameInput->setFocus();
}

void MainWindow::onShowLoginClicked()
{
    // Switch to login page
    stackedWidget->setCurrentWidget(loginPage);
}

void MainWindow::onLogoutButtonClicked()
{
    currentUser = "";
    usernameInput->clear();
    passwordInput->clear();
    loginStatusLabel->setText("Please log in to access the gym management system");
    stackedWidget->setCurrentWidget(loginPage);
}

void MainWindow::onMembersButtonClicked()
{
    updateMembersTable();
    stackedWidget->setCurrentWidget(membersPage);
}

void MainWindow::onWorkoutPlanButtonClicked()
{
    updateWorkoutTable();
    stackedWidget->setCurrentWidget(workoutPage);
}

void MainWindow::onCalorieCalculatorButtonClicked()
{
    stackedWidget->setCurrentWidget(caloriesPage);
}

void MainWindow::onBackToMainButtonClicked()
{
    stackedWidget->setCurrentWidget(dashboardPage);
}

void MainWindow::onBackFromWorkoutButtonClicked()
{
    stackedWidget->setCurrentWidget(dashboardPage);
}

void MainWindow::onBackFromCaloriesButtonClicked()
{
    stackedWidget->setCurrentWidget(dashboardPage);
}

void MainWindow::updateMembersTable()
{
    // Clear the table
    membersTable->setRowCount(0);

    // Populate the table with members
    for (int i = 0; i < members.size(); ++i) {
        membersTable->insertRow(i);
        membersTable->setItem(i, 0, new QTableWidgetItem(members[i].name));
        membersTable->setItem(i, 1, new QTableWidgetItem(members[i].phone));
        membersTable->setItem(i, 2, new QTableWidgetItem(members[i].email));
        membersTable->setItem(i, 3, new QTableWidgetItem(members[i].membershipType));
        membersTable->setItem(i, 4, new QTableWidgetItem(members[i].joinDate.toString("dd/MM/yyyy")));
        membersTable->setItem(i, 5, new QTableWidgetItem(members[i].expiryDate.toString("dd/MM/yyyy")));
    }
}

void MainWindow::updateWorkoutTable()
{
    // Clear the table
    workoutTable->setRowCount(0);

    // Populate the table with workouts
    for (int i = 0; i < workouts.size(); ++i) {
        workoutTable->insertRow(i);
        workoutTable->setItem(i, 0, new QTableWidgetItem(workouts[i].name));
        workoutTable->setItem(i, 1, new QTableWidgetItem(workouts[i].target));
        workoutTable->setItem(i, 2, new QTableWidgetItem(workouts[i].description));
        workoutTable->setItem(i, 3, new QTableWidgetItem(QString::number(workouts[i].sets)));
        workoutTable->setItem(i, 4, new QTableWidgetItem(QString::number(workouts[i].reps)));
    }
}

void MainWindow::onAddMemberButtonClicked()
{
    QString name = memberNameInput->text();
    QString phone = memberPhoneInput->text();
    QString email = memberEmailInput->text();
    QString membershipType = membershipTypeCombo->currentText();

    if (name.isEmpty() || phone.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please fill in all member details!");
        return;
    }

    Member newMember;
    newMember.name = name;
    newMember.phone = phone;
    newMember.email = email;
    newMember.membershipType = membershipType;
    newMember.joinDate = QDate::currentDate();

    // Set expiry date based on membership type
    if (membershipType == "Monthly") {
        newMember.expiryDate = newMember.joinDate.addMonths(1);
    } else if (membershipType == "Quarterly") {
        newMember.expiryDate = newMember.joinDate.addMonths(3);
    } else if (membershipType == "Annual") {
        newMember.expiryDate = newMember.joinDate.addYears(1);
    }

    members.append(newMember);
    updateMembersTable();

    // Clear inputs
    memberNameInput->clear();
    memberPhoneInput->clear();
    memberEmailInput->clear();

    QMessageBox::information(this, "Success", "Member added successfully!");
}

void MainWindow::onDeleteMemberButtonClicked()
{
    int selectedRow = membersTable->currentRow();
    if (selectedRow >= 0 && selectedRow < members.size()) {
        QString memberName = members[selectedRow].name;

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm Delete",
                                      "Are you sure you want to delete member: " + memberName + "?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            members.removeAt(selectedRow);
            updateMembersTable();
            QMessageBox::information(this, "Success", "Member deleted successfully!");
        }
    } else {
        QMessageBox::warning(this, "Selection Error", "Please select a member to delete!");
    }
}

void MainWindow::onEditMemberButtonClicked()
{
    int selectedRow = membersTable->currentRow();
    if (selectedRow >= 0 && selectedRow < members.size()) {
        QString name = memberNameInput->text();
        QString phone = memberPhoneInput->text();
        QString email = memberEmailInput->text();
        QString membershipType = membershipTypeCombo->currentText();

        if (name.isEmpty() || phone.isEmpty() || email.isEmpty()) {
            QMessageBox::warning(this, "Invalid Input", "Please fill in all member details!");
            return;
        }

        members[selectedRow].name = name;
        members[selectedRow].phone = phone;
        members[selectedRow].email = email;

        // Only update membership type and expiry if it changed
        if (members[selectedRow].membershipType != membershipType) {
            members[selectedRow].membershipType = membershipType;

            // Reset expiry date based on new membership type
            if (membershipType == "Monthly") {
                members[selectedRow].expiryDate = QDate::currentDate().addMonths(1);
            } else if (membershipType == "Quarterly") {
                members[selectedRow].expiryDate = QDate::currentDate().addMonths(3);
            } else if (membershipType == "Annual") {
                members[selectedRow].expiryDate = QDate::currentDate().addYears(1);
            }
        }

        updateMembersTable();
        QMessageBox::information(this, "Success", "Member updated successfully!");
    } else {
        QMessageBox::warning(this, "Selection Error", "Please select a member to edit!");
    }
}

void MainWindow::onAddWorkoutButtonClicked()
{
    QString name = workoutNameInput->text();
    QString target = workoutTargetInput->text();
    QString description = workoutDescInput->toPlainText();
    int sets = workoutSetsInput->value();
    int reps = workoutRepsInput->value();

    if (name.isEmpty() || target.isEmpty() || description.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please fill in all workout details!");
        return;
    }

    Workout newWorkout;
    newWorkout.name = name;
    newWorkout.target = target;
    newWorkout.description = description;
    newWorkout.sets = sets;
    newWorkout.reps = reps;

    workouts.append(newWorkout);
    updateWorkoutTable();

    // Clear inputs
    workoutNameInput->clear();
    workoutTargetInput->clear();
    workoutDescInput->clear();
    workoutSetsInput->setValue(3);
    workoutRepsInput->setValue(10);

    QMessageBox::information(this, "Success", "Workout added successfully!");
}

void MainWindow::onDeleteWorkoutButtonClicked()
{
    int selectedRow = workoutTable->currentRow();
    if (selectedRow >= 0 && selectedRow < workouts.size()) {
        QString workoutName = workouts[selectedRow].name;

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm Delete",
                                      "Are you sure you want to delete workout: " + workoutName + "?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            workouts.removeAt(selectedRow);
            updateWorkoutTable();
            QMessageBox::information(this, "Success", "Workout deleted successfully!");
        }
    } else {
        QMessageBox::warning(this, "Selection Error", "Please select a workout to delete!");
    }
}

void MainWindow::onGenderSelectionChanged()
{
    // Can be used to dynamically update UI based on gender selection
    // For now, we'll just use this in the calculation
}

void MainWindow::onActivityLevelChanged(int index)
{
    // Update activity description based on selection
    switch (index) {
    case 0:
        activityDescriptionLabel->setText("Little or no exercise, desk job");
        break;
    case 1:
        activityDescriptionLabel->setText("Light exercise 1-3 days/week");
        break;
    case 2:
        activityDescriptionLabel->setText("Moderate exercise 3-5 days/week");
        break;
    case 3:
        activityDescriptionLabel->setText("Hard exercise 6-7 days/week");
        break;
    case 4:
        activityDescriptionLabel->setText("Very hard exercise, physical job or training twice a day");
        break;
    }
}

double MainWindow::calculateBMR()
{
    double weight = weightInput->value();
    double height = heightInput->value();
    int age = ageInput->value();
    bool isMale = maleRadio->isChecked();

    // Mifflin-St Jeor Equation for BMR
    if (isMale) {
        return (10 * weight) + (6.25 * height) - (5 * age) + 5;
    } else {
        return (10 * weight) + (6.25 * height) - (5 * age) - 161;
    }
}

double MainWindow::calculateTDEE(double bmr)
{
    double activityFactor = 1.2; // Default to sedentary

    // Set activity factor based on selection
    switch (activityLevelCombo->currentIndex()) {
    case 0: // Sedentary
        activityFactor = 1.2;
        break;
    case 1: // Lightly active
        activityFactor = 1.375;
        break;
    case 2: // Moderately active
        activityFactor = 1.55;
        break;
    case 3: // Very active
        activityFactor = 1.725;
        break;
    case 4: // Extra active
        activityFactor = 1.9;
        break;
    }

    return bmr * activityFactor;
}

void MainWindow::onCalculateCaloriesButtonClicked()
{
    double bmr = calculateBMR();
    double tdee = calculateTDEE(bmr);

    // Update the result labels
    bmrResultLabel->setText(QString::number(qRound(bmr)) + " calories/day");
    tdeeResultLabel->setText(QString::number(qRound(tdee)) + " calories/day");

    // Calculate weight loss, maintenance, and gain calories
    double weightLoss = tdee - 500; // 500 calorie deficit for 0.5kg/week loss
    double weightGain = tdee + 500; // 500 calorie surplus for 0.5kg/week gain

    weightLossLabel->setText(QString::number(qRound(weightLoss)) + " calories/day");
    maintainWeightLabel->setText(QString::number(qRound(tdee)) + " calories/day");
    weightGainLabel->setText(QString::number(qRound(weightGain)) + " calories/day");

    QMessageBox::information(this, "Calculation Complete",
                             "Calorie calculations have been completed.\n\n"
                             "BMR: " + QString::number(qRound(bmr)) + " calories/day\n"
                                                                  "TDEE: " + QString::number(qRound(tdee)) + " calories/day");
}
