#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include<qcoreapplication.h>
#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <qcoreapplication.h>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QKeyEvent>
#include <QInputDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(900, 600);//set the initial
    // Set up the central widget and stacked widget for page switching
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    weightHistoryFilePath = QCoreApplication::applicationDirPath() + "/weight_history.csv";
    foodsFilePath = QCoreApplication::applicationDirPath() + "/foods.csv";
    foodDiaryFilePath = QCoreApplication::applicationDirPath() + "/food_diary.csv";
    loadFoodsFromFile();
    loadFoodDiaryFromFile();

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackedWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setupLoginPage();
    setupSignUpPage();
    setupDashboardPage();
    setupMembersPage();
    setupWorkoutPage();
    setupCaloriesPage();

    Food apple;
    apple.name = "Apple";
    apple.calories = 95;
    apple.protein = 0.5;
    apple.carbs = 25;
    apple.fat = 0.3;
    foods.append(apple);

    Food chickenBreast;
    chickenBreast.name = "Chicken Breast";
    chickenBreast.calories = 165;
    chickenBreast.protein = 31;
    chickenBreast.carbs = 0;
    chickenBreast.fat = 3.6;
    foods.append(chickenBreast);

    Food rice;
    rice.name = "White Rice";
    rice.calories = 205;
    rice.protein = 4.3;
    rice.carbs = 45;
    rice.fat = 0.4;
    foods.append(rice);

    Food banana;
    banana.name = "Banana";
    banana.calories = 105;
    banana.protein = 1.3;
    banana.carbs = 27;
    banana.fat = 0.4;
    foods.append(banana);

    Food eggs;
    eggs.name = "Eggs (2 large)";
    eggs.calories = 140;
    eggs.protein = 12;
    eggs.carbs = 1;
    eggs.fat = 10;
    foods.append(eggs);

    // Add default admin account
    dataFilePath = QCoreApplication::applicationDirPath() + "/loginpage.csv";
    if (QFile::exists(dataFilePath)) {
        loadMembersFromFile(dataFilePath);
    } else {
        // Add default admin account
        userCredentials["admin"] = "password";

        // Only load sample data if no file exists
        loadSampleData();
    }

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

    setupWeightTrackingPage();
    stackedWidget->addWidget(weightTrackingPage);
    loadWeightHistoryFromFile();
    updateWeightTable();
    updateWeightChart();
}

MainWindow::~MainWindow()
{
    saveFoodsToFile();
    saveFoodDiaryToFile();
    saveMembersToFile(dataFilePath);
    saveWeightHistoryToFile();
}

void MainWindow::loadSampleData()
{
    // Sample members
    Member member1;
    member1.name = "Advik Rajvansh";
    member1.phone = "9899490181";
    member1.email = "advikrajvansh@gmail.com";
    member1.username = "advik121";
    member1.password = "advik2512";
    member1.membershipType = "Annual";
    member1.joinDate = QDate::currentDate().addMonths(-3);
    member1.expiryDate = member1.joinDate.addYears(1);
    members.append(member1);
    userCredentials[member1.username] = member1.password;

    Member member2;
    member2.name = "Tushar Tandon";
    member2.phone = "9870369632";
    member2.email = "tushar@example.com";
    member2.username = "chus121";
    member2.password = "chus121";
    member2.membershipType = "Monthly";
    member2.joinDate = QDate::currentDate().addMonths(-1);
    member2.expiryDate = member2.joinDate.addMonths(1);
    members.append(member2);
    userCredentials[member2.username] = member2.password;


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
void MainWindow::saveFoodsToFile() {
    QFile file(foodsFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    out << "Name,Calories,Protein,Carbs,Fat\n";
    for (int i = 0; i < foods.size(); ++i) {
        const Food &food = foods.at(i);
        out << food.name << "," << food.calories << "," << food.protein << "," << food.carbs << "," << food.fat << "\n";
    }
    file.close();
}
void MainWindow::loadFoodsFromFile() {
    QFile file(foodsFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    if (!in.atEnd()) in.readLine(); // skip header
    foods.clear();
    while (!in.atEnd()) {
        QStringList fields = in.readLine().split(",");
        if (fields.size() == 5) {
            Food food{fields[0], fields[1].toDouble(), fields[2].toDouble(), fields[3].toDouble(), fields[4].toDouble()};
            foods.append(food);
        }
    }
    file.close();
}
void MainWindow::saveFoodDiaryToFile() {
    QFile file(foodDiaryFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    out << "Date,Meal,Food,Calories,Protein,Carbs,Fat\n";
    for (auto dateIt = foodDiary.begin(); dateIt != foodDiary.end(); ++dateIt) {
        QDate date = dateIt.key();
        for (auto mealIt = dateIt.value().begin(); mealIt != dateIt.value().end(); ++mealIt) {
            QString meal = mealIt.key();
            const auto &mealFoods = mealIt.value();
            for (const Food &food : mealFoods)
                out << date.toString(Qt::ISODate) << "," << meal << "," << food.name << "," << food.calories << "," << food.protein << "," << food.carbs << "," << food.fat << "\n";
        }
    }
    file.close();
}
void MainWindow::loadFoodDiaryFromFile() {
    QFile file(foodDiaryFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    if (!in.atEnd()) in.readLine(); // skip header
    foodDiary.clear();
    while (!in.atEnd()) {
        QStringList fields = in.readLine().split(",");
        if (fields.size() == 7) {
            QDate date = QDate::fromString(fields[0], Qt::ISODate);
            QString meal = fields[1];
            Food food{fields[2], fields[3].toDouble(), fields[4].toDouble(), fields[5].toDouble(), fields[6].toDouble()};
            foodDiary[date][meal].append(food);
        }
    }
    file.close();
}
void MainWindow::onFoodSearchTextChanged(const QString &text) {
    QListWidget* foodSearchResults = findChild<QListWidget*>("foodSearchResults");
    if (!foodSearchResults) return;
    foodSearchResults->clear();
    for (int i = 0; i < foods.size(); ++i) {
        const Food &food = foods.at(i);
        if (food.name.contains(text, Qt::CaseInsensitive)) {
            QString displayText = QString("%1 (%2 kcal, P:%3g, C:%4g, F:%5g)")
            .arg(food.name).arg(food.calories).arg(food.protein).arg(food.carbs).arg(food.fat);
            QListWidgetItem* item = new QListWidgetItem(displayText);
            item->setData(Qt::UserRole, QVariant::fromValue(food));
            foodSearchResults->addItem(item);
        }
    }
}
void MainWindow::onFoodSelected(QListWidgetItem* item) {
    if (!item) return;
    Food selectedFood = item->data(Qt::UserRole).value<Food>();
    QDateEdit* diaryDateEdit = findChild<QDateEdit*>("diaryDateEdit");
    QComboBox* mealCombo = findChild<QComboBox*>("mealCombo");
    QTableWidget* diaryTable = findChild<QTableWidget*>("diaryTable");
    if (!diaryDateEdit || !mealCombo || !diaryTable) return;
    QDate date = diaryDateEdit->date();
    QString meal = mealCombo->currentText();
    foodDiary[date][meal].append(selectedFood);
    int row = diaryTable->rowCount();
    diaryTable->insertRow(row);
    diaryTable->setItem(row, 0, new QTableWidgetItem(selectedFood.name));
    diaryTable->setItem(row, 1, new QTableWidgetItem(QString::number(selectedFood.calories)));
    diaryTable->setItem(row, 2, new QTableWidgetItem(QString::number(selectedFood.protein)));
    diaryTable->setItem(row, 3, new QTableWidgetItem(QString::number(selectedFood.carbs)));
    diaryTable->setItem(row, 4, new QTableWidgetItem(QString::number(selectedFood.fat)));
    updateNutritionSummary();
    saveFoodDiaryToFile();
}
void MainWindow::updateNutritionSummary() {
    QDateEdit* diaryDateEdit = findChild<QDateEdit*>("diaryDateEdit");
    QComboBox* mealCombo = findChild<QComboBox*>("mealCombo");
    QLabel* caloriesLabel = findChild<QLabel*>("caloriesLabel");
    QLabel* proteinLabel = findChild<QLabel*>("proteinLabel");
    QLabel* carbsLabel = findChild<QLabel*>("carbsLabel");
    QLabel* fatLabel = findChild<QLabel*>("fatLabel");
    QProgressBar* calorieProgress = findChild<QProgressBar*>("calorieProgress");
    QSpinBox* goalSpin = findChild<QSpinBox*>("goalSpin");
    if (!diaryDateEdit || !mealCombo || !caloriesLabel || !proteinLabel || !carbsLabel || !fatLabel || !calorieProgress || !goalSpin) return;
    QDate date = diaryDateEdit->date();
    QString meal = mealCombo->currentText();
    double totalCalories = 0, totalProtein = 0, totalCarbs = 0, totalFat = 0;
    const auto &mealList = foodDiary[date][meal];
    for (int i = 0; i < mealList.size(); ++i) {
        const Food &food = mealList.at(i);
        totalCalories += food.calories;
        totalProtein += food.protein;
        totalCarbs += food.carbs;
        totalFat += food.fat;
    }
    caloriesLabel->setText(QString("Calories: %1 kcal").arg(totalCalories));
    proteinLabel->setText(QString("Protein: %1 g").arg(totalProtein));
    carbsLabel->setText(QString("Carbs: %1 g").arg(totalCarbs));
    fatLabel->setText(QString("Fat: %1 g").arg(totalFat));
    calorieProgress->setValue(totalCalories);
    calorieProgress->setMaximum(goalSpin->value());
    updateMacroChart(totalProtein, totalCarbs, totalFat);
}
void MainWindow::updateMacroChart(double protein, double carbs, double fat) {
    QChartView* macroChartView = findChild<QChartView*>("macroChartView");
    if (!macroChartView) return;
    QChart* chart = macroChartView->chart();
    if (!chart) return;
    auto seriesList = chart->series();
    if (seriesList.isEmpty()) return;
    QPieSeries* series = qobject_cast<QPieSeries*>(seriesList.first());
    if (!series) return;
    series->clear();
    series->append("Protein", protein);
    series->append("Carbs", carbs);
    series->append("Fat", fat);
}
void MainWindow::setupWeightTrackingPage()
{
    weightTrackingPage = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(weightTrackingPage);

    // Set background gradient
    weightTrackingPage->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #232526, stop:1 #414345);"
        );

    // Title
    QLabel* titleLabel = new QLabel("Weight Tracking", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #00bfff; margin-bottom: 24px;");

    // Back button
    backFromWeightButton = new QPushButton("← Back", this);
    backFromWeightButton->setFixedSize(90, 36);
    backFromWeightButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #222;"
        "  color: #00bfff;"
        "  border-radius: 8px;"
        "  font-weight: bold;"
        "  font-size: 16px;"
        "  padding: 4px 12px;"
        "  margin-bottom: 8px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #00bfff;"
        "  color: #fff;"
        "}"
        );

    // Input section
    QGroupBox* inputBox = new QGroupBox("Add Weight Entry", this);
    inputBox->setStyleSheet(
        "QGroupBox {"
        "  color: #fff;"
        "  background: rgba(0,0,0,0.2);"
        "  border: 1px solid #fff;"
        "  border-radius: 8px;"
        "  margin-top: 20px;"
        "}"
        );

    QFormLayout* inputLayout = new QFormLayout(inputBox);
    weightDateEdit = new QDateEdit(QDate::currentDate(), this);
    weightDateEdit->setCalendarPopup(true);
    weightDateEdit->setStyleSheet("background: rgba(255,255,255,0.1); color: #fff; border: 1px solid #fff; border-radius: 4px;");

    weightInput = new QDoubleSpinBox(this);
    weightInput->setRange(30, 300);
    weightInput->setSuffix(" kg");
    weightInput->setValue(70);
    weightInput->setStyleSheet("background: rgba(255,255,255,0.1); color: #fff; border: 1px solid #fff; border-radius: 4px;");

    addWeightButton = new QPushButton("Add Weight", this);
    addWeightButton->setStyleSheet(
        "QPushButton {"
        "  background: #00bfff;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding: 8px;"
        "}"
        "QPushButton:hover { background: #0099cc; }"
        );

    inputLayout->addRow("Date:", weightDateEdit);
    inputLayout->addRow("Weight:", weightInput);
    inputLayout->addRow(addWeightButton);

    // Weight table
    weightTable = new QTableWidget(0, 2, this);
    weightTable->setHorizontalHeaderLabels({"Date", "Weight (kg)"});
    weightTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    weightTable->setStyleSheet(
        "QTableWidget {"
        "  background: rgba(255,255,255,0.1);"
        "  color: #fff;"
        "  border: 1px solid #fff;"
        "  border-radius: 4px;"
        "}"
        "QHeaderView::section {"
        "  background: rgba(0,0,0,0.3);"
        "  color: #fff;"
        "  padding: 6px;"
        "  border: none;"
        "}"
        );

    deleteWeightButton = new QPushButton("Delete Selected", this);
    deleteWeightButton->setStyleSheet(
        "QPushButton {"
        "  background: #ff4444;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding: 8px;"
        "}"
        "QPushButton:hover { background: #cc0000; }"
        );

    // Weight chart
    weightChartView = new QChartView(this);
    weightChartView->setStyleSheet("background: transparent;");
    weightChartView->setRenderHint(QPainter::Antialiasing);

    // Layout organization
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(inputBox, 1);
    topLayout->addWidget(weightChartView, 2);

    mainLayout->addWidget(backFromWeightButton, 0, Qt::AlignLeft);
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(weightTable);
    mainLayout->addWidget(deleteWeightButton);

    // Connect signals and slots
    connect(backFromWeightButton, &QPushButton::clicked, this, &MainWindow::onBackFromWeightButtonClicked);
    connect(addWeightButton, &QPushButton::clicked, this, &MainWindow::onAddWeightButtonClicked);
    connect(deleteWeightButton, &QPushButton::clicked, this, &MainWindow::onDeleteWeightButtonClicked);
    connect(weightTable, &QTableWidget::itemSelectionChanged, this, [this]() {
        deleteWeightButton->setEnabled(weightTable->currentRow() >= 0);
    });

    // Initial state
    deleteWeightButton->setEnabled(false);
    updateWeightTable();
    updateWeightChart();
}



void MainWindow::setupLoginPage()
{
    // Create login page widget
    loginPage = new QWidget(this);

    // Set background image
    loginPage->setStyleSheet(
        "background-image: url(:/resources/gym.jpg);"
        "background-repeat: no-repeat;"
        "background-position: center;"
        "background-size: cover;"
        );

    // Outer layout for centering
    QVBoxLayout* outerLayout = new QVBoxLayout(loginPage);
    outerLayout->setAlignment(Qt::AlignCenter);

    // Semi-transparent overlay for the form
    QWidget* overlay = new QWidget(loginPage);
    overlay->setMaximumWidth(500); // Responsive width
    overlay->setStyleSheet(
        "background: rgba(0, 0, 0, 0.4);"
        "border-radius: 20px;"
        "box-shadow: 0 8px 32px 0 rgba(0,0,0,0.37);"
        );

    // The actual login form layout inside the overlay
    QGridLayout* loginLayout = new QGridLayout(overlay);

    // Create title
    loginTitleLabel = new QLabel("FitLife", overlay);
    QFont titleFont = loginTitleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    loginTitleLabel->setFont(titleFont);
    loginTitleLabel->setAlignment(Qt::AlignCenter);

    // Create username components
    usernameLabel = new QLabel("Username:", overlay);
    usernameInput = new QLineEdit(overlay);
    usernameInput->setPlaceholderText("Enter your username");
    usernameInput->setStyleSheet(
        "QLineEdit {"
        "background: rgba(0,0,0,0.2);"
        "color: #fff;"
        "border: 1px solid #fff;"
        "border-radius: 6px;"
        "padding: 6px;"
        "font-size: 15px;"
        "}"
        );

    // Create password components
    passwordLabel = new QLabel("Password:", overlay);
    passwordInput = new QLineEdit(overlay);
    passwordInput->setPlaceholderText("Enter your password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet(usernameInput->styleSheet());

    // Create buttons
    loginButton = new QPushButton(QIcon(":/resources/login.png"), "Login", overlay);
    clearLoginButton = new QPushButton(QIcon(":/resources/clear.png"), "Clear", overlay);
    showSignUpButton = new QPushButton("Create New Account", overlay);
    QString buttonStyle =
        "QPushButton {"
        "background: rgba(0,0,0,0.3);"
        "color: #fff;"
        "border-radius: 8px;"
        "padding: 10px 0;"
        "font-size: 16px;"
        "border: 1px solid #fff;"
        "}"
        "QPushButton:hover {"
        "background: rgba(0,0,0,0.5);"
        "}";
    loginButton->setStyleSheet(buttonStyle);
    clearLoginButton->setStyleSheet(buttonStyle);
    showSignUpButton->setStyleSheet(buttonStyle);

    // Create status label
    loginStatusLabel = new QLabel("Please log in to access the gym management system", overlay);
    loginStatusLabel->setAlignment(Qt::AlignCenter);
    loginTitleLabel->setStyleSheet("background: transparent; color: #fff; font-size: 32px; font-weight: bold;");
    loginStatusLabel->setStyleSheet("background: transparent; color: #fff;");
    usernameLabel->setStyleSheet("background: transparent; color: #fff;");
    passwordLabel->setStyleSheet("background: transparent; color: #fff;");

    // Add widgets to layout
    loginLayout->addWidget(loginTitleLabel, 0, 0, 1, 2);
    loginLayout->addWidget(new QLabel("Login to Your Account", overlay), 1, 0, 1, 2, Qt::AlignCenter);
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

    // Add the overlay to the outer layout, centered
    outerLayout->addStretch(1);
    outerLayout->addWidget(overlay, 0, Qt::AlignCenter);
    outerLayout->addStretch(1);

    // Set the layout for the login page
    loginPage->setLayout(outerLayout);

    // Connect signals and slots
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginButtonClicked);
    connect(clearLoginButton, &QPushButton::clicked, this, &MainWindow::onClearLoginButtonClicked);
    connect(showSignUpButton, &QPushButton::clicked, this, &MainWindow::onShowSignUpClicked);
    loginButton->setDefault(true);
    connect(passwordInput, &QLineEdit::returnPressed, this, &MainWindow::onLoginButtonClicked);
    connect(usernameInput, &QLineEdit::returnPressed, this, &MainWindow::onLoginButtonClicked);
}

// Save members list to a text file
// Save members list to a text file
void MainWindow::saveMembersToFile(const QString& filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int i = 0; i < members.size(); ++i) {
            const Member &member = members[i];
            out << member.name << "," << member.phone << "," << member.email << ","
                << member.username << "," << member.password << ","
                << member.membershipType << "," << member.joinDate.toString(Qt::ISODate) << ","
                << member.expiryDate.toString(Qt::ISODate) << "\n";

            // Make sure username/password is in credentials map
            userCredentials[member.username] = member.password;
        }
        file.close();
        qDebug() << "Member data saved successfully to " + filePath;
    } else {
        QMessageBox::warning(nullptr, "File Error", "Unable to save members data to file: " + filePath + "\nError: " + file.errorString());
        qDebug() << "Unable to save members data to file: " + filePath + "\nError: " + file.errorString();
    }
}
void MainWindow::saveWeightHistoryToFile() {
    QFile file(weightHistoryFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    out << "Date,Weight\n";
    for (int i = 0; i < weightHistory.size(); ++i) {
        out << weightHistory[i].first.toString(Qt::ISODate) << "," << weightHistory[i].second << "\n";
    }
    file.close();
}

void MainWindow::loadWeightHistoryFromFile() {
    QFile file(weightHistoryFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    if (!in.atEnd()) in.readLine(); // skip header
    weightHistory.clear();
    while (!in.atEnd()) {
        QStringList fields = in.readLine().split(",");
        if (fields.size() == 2) {
            QDate date = QDate::fromString(fields[0], Qt::ISODate);
            double weight = fields[1].toDouble();
            if (date.isValid())
                weightHistory.append(qMakePair(date, weight));
        }
    }
    file.close();
}

// Load members list from a text file
void MainWindow::loadMembersFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        members.clear(); // Clear existing data
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields.size() == 8) {
                Member m;
                m.name = fields[0];
                m.phone = fields[1];
                m.email = fields[2];
                m.username = fields[3];
                m.password = fields[4];
                m.membershipType = fields[5];
                m.joinDate = QDate::fromString(fields[6], Qt::ISODate);
                m.expiryDate = QDate::fromString(fields[7], Qt::ISODate);
                members.append(m);

                // Also add this user to login credentials if it doesn't exist
                userCredentials[m.username] = m.password;
                qDebug() << "Loaded user: " << m.username;
            }
        }
        file.close();
        updateMembersTable(); // Refresh table
        qDebug() << "Member data loaded successfully from " + filePath;
    } else {
        qDebug() << "Unable to load members data from file: " + filePath + "\nError: " + file.errorString();
    }
}
void MainWindow::setupSignUpPage()
{
    // Create sign up page widget
    signUpPage = new QWidget(this);

    // Set background image
    signUpPage->setStyleSheet(
        "background-image: url(:/resources/gym.jpg);"
        "background-repeat: no-repeat;"
        "background-position: center;"
        "background-size: cover;"
        );

    // Outer layout for centering
    QVBoxLayout* outerLayout = new QVBoxLayout(signUpPage);
    outerLayout->setAlignment(Qt::AlignCenter);

    // Semi-transparent overlay for the form
    QWidget* overlay = new QWidget(signUpPage);
    overlay->setMaximumWidth(750);
    overlay->setStyleSheet(
        "background: rgba(0, 0, 0, 0.4);"
        "border-radius: 20px;"
        "box-shadow: 0 8px 32px 0 rgba(0,0,0,0.37);"
        );

    // The actual sign up form layout inside the overlay
    QGridLayout* signUpLayout = new QGridLayout(overlay);

    // Title
    QLabel* signUpTitleLabel = new QLabel("FitLife Gym", overlay);
    QFont titleFont = signUpTitleLabel->font();
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    signUpTitleLabel->setFont(titleFont);
    signUpTitleLabel->setAlignment(Qt::AlignCenter);
    signUpTitleLabel->setStyleSheet("background: transparent; color: #fff; font-size: 32px; font-weight: bold;");

    QLabel* subTitle = new QLabel("Create New Account", overlay);
    QFont subFont = subTitle->font();
    subFont.setPointSize(14);
    subTitle->setFont(subFont);
    subTitle->setAlignment(Qt::AlignCenter);
    subTitle->setStyleSheet("background: transparent; color: #fff; margin-bottom: 18px;");

    // Username
    newUsernameLabel = new QLabel("Username:", overlay);
    newUsernameInput = new QLineEdit(overlay);
    newUsernameInput->setPlaceholderText("Choose a username");
    newUsernameInput->setStyleSheet(
        "QLineEdit {"
        "background: rgba(0,0,0,0.2);"
        "color: #fff;"
        "border: 1px solid #fff;"
        "border-radius: 6px;"
        "padding: 6px;"
        "font-size: 15px;"
        "}"
        );
    newUsernameLabel->setStyleSheet("background: transparent; color: #fff;");

    // Password
    newPasswordLabel = new QLabel("Password:", overlay);
    newPasswordInput = new QLineEdit(overlay);
    newPasswordInput->setPlaceholderText("Create a password");
    newPasswordInput->setEchoMode(QLineEdit::Password);
    newPasswordInput->setStyleSheet(newUsernameInput->styleSheet());
    newPasswordLabel->setStyleSheet("background: transparent; color: #fff;");

    // Confirm Password
    confirmPasswordLabel = new QLabel("Confirm Password:", overlay);
    confirmPasswordInput = new QLineEdit(overlay);
    confirmPasswordInput->setPlaceholderText("Confirm your password");
    confirmPasswordInput->setEchoMode(QLineEdit::Password);
    confirmPasswordInput->setStyleSheet(newUsernameInput->styleSheet());
    confirmPasswordLabel->setStyleSheet("background: transparent; color: #fff;");

    // Email
    emailLabel = new QLabel("Email:", overlay);
    emailInput = new QLineEdit(overlay);
    emailInput->setPlaceholderText("Enter your email");
    emailInput->setStyleSheet(newUsernameInput->styleSheet());
    emailLabel->setStyleSheet("background: transparent; color: #fff;");

    // Buttons
    signUpButton = new QPushButton(QIcon(":/resources/register.png"), "Register", overlay);
    clearSignUpButton = new QPushButton(QIcon(":/resources/clear.png"), "Clear", overlay);
    QString buttonStyle =
        "QPushButton {"
        "background: rgba(0,0,0,0.3);"
        "color: #fff;"
        "border-radius: 8px;"
        "padding: 10px 0;"
        "font-size: 16px;"
        "border: 1px solid #fff;"
        "}"
        "QPushButton:hover {"
        "background: rgba(0,0,0,0.5);"
        "}";
    signUpButton->setStyleSheet(buttonStyle);
    clearSignUpButton->setStyleSheet(buttonStyle);

    // Back to login
    showLoginButton = new QPushButton("Back to Login", overlay);
    showLoginButton->setStyleSheet(buttonStyle);

    // Status label
    signUpStatusLabel = new QLabel("Please fill in all fields to create your account", overlay);
    signUpStatusLabel->setAlignment(Qt::AlignCenter);
    signUpStatusLabel->setStyleSheet("background: transparent; color: #fff;");

    // Add widgets to layout
    signUpLayout->addWidget(signUpTitleLabel, 0, 0, 1, 2);
    signUpLayout->addWidget(subTitle, 1, 0, 1, 2, Qt::AlignCenter);
    signUpLayout->addWidget(newUsernameLabel, 2, 0);
    signUpLayout->addWidget(newUsernameInput, 2, 1);
    signUpLayout->addWidget(newPasswordLabel, 3, 0);
    signUpLayout->addWidget(newPasswordInput, 3, 1);
    signUpLayout->addWidget(confirmPasswordLabel, 4, 0);
    signUpLayout->addWidget(confirmPasswordInput, 4, 1);
    signUpLayout->addWidget(emailLabel, 5, 0);
    signUpLayout->addWidget(emailInput, 5, 1);

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(signUpButton);
    buttonLayout->addWidget(clearSignUpButton);

    signUpLayout->addLayout(buttonLayout, 6, 0, 1, 2);
    signUpLayout->addWidget(showLoginButton, 7, 0, 1, 2);
    signUpLayout->addWidget(signUpStatusLabel, 8, 0, 1, 2);

    // Set some spacing and margins
    signUpLayout->setSpacing(15);
    signUpLayout->setContentsMargins(40, 40, 40, 40);

    // Add the overlay to the outer layout, centered
    outerLayout->addStretch(1);
    outerLayout->addWidget(overlay, 0, Qt::AlignCenter);
    outerLayout->addStretch(1);

    // Connect signals and slots
    connect(signUpButton, &QPushButton::clicked, this, &MainWindow::onSignUpButtonClicked);
    connect(clearSignUpButton, &QPushButton::clicked, this, &MainWindow::onClearSignUpButtonClicked);
    connect(showLoginButton, &QPushButton::clicked, this, &MainWindow::onShowLoginClicked);
    connect(newUsernameInput, &QLineEdit::returnPressed, this, &MainWindow::onSignUpButtonClicked);
    connect(newPasswordInput, &QLineEdit::returnPressed, this, &MainWindow::onSignUpButtonClicked);
    connect(confirmPasswordInput, &QLineEdit::returnPressed, this, &MainWindow::onSignUpButtonClicked);
    connect(emailInput, &QLineEdit::returnPressed, this, &MainWindow::onSignUpButtonClicked);
}

void MainWindow::setupDashboardPage()
{
    dashboardPage = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(dashboardPage);
    welcomeLabel = new QLabel(this); // or with your rich text, etc.
    mainLayout->addWidget(welcomeLabel);

    // Set background image or gradient
    dashboardPage->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #232526, stop:1 #414345);"
        );

    // Welcome label
    QLabel* welcomeLabel = new QLabel(
        QString("Welcome to <span style='color:#00bfff;'>FitLife Progress Tracking System</span><b>%1</b>!")
            .arg(currentUser), this);
    welcomeLabel->setTextFormat(Qt::RichText);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    QFont welcomeFont = welcomeLabel->font();
    welcomeFont.setPointSize(26);
    welcomeFont.setBold(true);
    welcomeLabel->setFont(welcomeFont);
    welcomeLabel->setStyleSheet("color: white; text-shadow: 2px 2px 8px #000; margin-bottom: 40px;");

    mainLayout->addWidget(welcomeLabel);

    // Button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    // Card-style buttons with icons
    QPushButton* membersBtn = new QPushButton(QIcon(":/resources/members.png"), "Manage Members", this);
    QPushButton* workoutBtn = new QPushButton(QIcon(":/resources/workout.png"), "Workout Plans", this);
    QPushButton* calorieBtn = new QPushButton(QIcon(":/resources/calorie.png"), "Calorie Calculator", this);
    QPushButton* weightBtn = new QPushButton(QIcon(":/resources/weight.png"), "Weight Tracking", this);

    QString cardStyle =
        "QPushButton {"
        "background: rgba(255,255,255,0.08);"
        "color: #fff;"
        "border-radius: 18px;"
        "padding: 30px 40px;"
        "font-size: 20px;"
        "font-weight: 600;"
        "box-shadow: 0 4px 24px rgba(0,0,0,0.2);"
        "border: 2px solid #00bfff;"
        "margin: 0 20px;"
        "}"
        "QPushButton:hover {"
        "background: #00bfff;"
        "color: #232526;"
        "}";
    membersBtn->setStyleSheet(cardStyle);
    workoutBtn->setStyleSheet(cardStyle);
    calorieBtn->setStyleSheet(cardStyle);
    weightBtn->setStyleSheet(cardStyle);

    membersBtn->setIconSize(QSize(40, 40));
    workoutBtn->setIconSize(QSize(40, 40));
    calorieBtn->setIconSize(QSize(40, 40));
    weightBtn->setIconSize(QSize(40, 40));

    buttonLayout->addWidget(membersBtn);
    buttonLayout->addWidget(workoutBtn);
    buttonLayout->addWidget(calorieBtn);
     buttonLayout->addWidget(weightBtn);

    mainLayout->addLayout(buttonLayout);

    // Logout button
    QPushButton* logoutBtn = new QPushButton("Logout", this);
    logoutBtn->setStyleSheet(
        "QPushButton {"
        "background: #333;"
        "color: #fff;"
        "border-radius: 8px;"
        "padding: 8px 24px;"
        "font-size: 14px;"
        "margin-top: 40px;"
        "}"
        "QPushButton:hover {"
        "background: #ff4d4d;"
        "}"
        );
    mainLayout->addWidget(logoutBtn, 0, Qt::AlignRight);

    // Connect signals and slots
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutButtonClicked);
    connect(membersBtn, &QPushButton::clicked, this, &MainWindow::onMembersButtonClicked);
    connect(workoutBtn, &QPushButton::clicked, this, &MainWindow::onWorkoutPlanButtonClicked);
    connect(calorieBtn, &QPushButton::clicked, this, &MainWindow::onCalorieCalculatorButtonClicked);
    connect(weightBtn, &QPushButton::clicked, this, &MainWindow::onWeightTrackingButtonClicked);
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
        QVBoxLayout* mainLayout = new QVBoxLayout(workoutPage);


        // Set background gradient
        workoutPage->setStyleSheet(
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #232526, stop:1 #414345);"
            );

        // Title
        QLabel* titleLabel = new QLabel("Workout Tracker", this);
        QFont titleFont = titleLabel->font();
        titleFont.setPointSize(24);
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("color: #00bfff; margin-bottom: 24px;");
        QPushButton* backFromWorkoutButton = new QPushButton("← Back", this);
        backFromWorkoutButton->setFixedSize(90, 36);
        backFromWorkoutButton->setStyleSheet(
            "QPushButton {"
            "  background-color: #222;"
            "  color: #00bfff;"
            "  border-radius: 8px;"
            "  font-weight: bold;"
            "  font-size: 16px;"
            "  padding: 4px 12px;"
            "  margin-bottom: 8px;"
            "}"
            "QPushButton:hover {"
            "  background-color: #00bfff;"
            "  color: #fff;"
            "}"
         );
        mainLayout->addWidget(titleLabel);

        // Layouts
        QHBoxLayout* topLayout = new QHBoxLayout();

        // --- Left: Workout Input Form ---
        QLabel* addWorkoutTitle = new QLabel("Add New Workout", this);
        QFont addWorkoutFont = addWorkoutTitle->font();
        addWorkoutFont.setPointSize(16);
        addWorkoutFont.setBold(true);
        addWorkoutTitle->setFont(addWorkoutFont);
        addWorkoutTitle->setStyleSheet("color: #00bfff; margin-bottom: 8px;");
        QGroupBox* inputBox = new QGroupBox("", this);
        inputBox->setStyleSheet("color: #fff; background: rgba(0,0,0,0.2); border: 1px solid #fff; border-radius: 8px;");

        QVBoxLayout* inputSectionLayout = new QVBoxLayout();
        inputSectionLayout->addWidget(addWorkoutTitle);
        inputSectionLayout->addWidget(inputBox);
        inputSectionLayout->setSpacing(8);
        inputSectionLayout->setContentsMargins(0, 0, 0, 0);
        // Workout name input
        workoutNameInput = new QLineEdit(this);
        workoutNameInput->setPlaceholderText("Enter workout name");
        workoutNameInput->setStyleSheet("background: rgba(255,255,255,0.1); color: #fff; border: 1px solid #fff; border-radius: 4px; padding: 6px;");

        // Target muscle group
        workoutTargetInput = new QComboBox(this);
        workoutTargetInput->addItems({"Chest", "Back", "Legs", "Shoulders", "Arms", "Core", "Full Body"});
        workoutTargetInput->setStyleSheet("background: rgba(255,255,255,0.1); color: #fff; border: 1px solid #fff; border-radius: 4px; padding: 6px;");

        // Description
        workoutDescInput = new QTextEdit(this);
        workoutDescInput->setPlaceholderText("Enter workout description");
        workoutDescInput->setStyleSheet("background: rgba(255,255,255,0.1); color: #fff; border: 1px solid #fff; border-radius: 4px; padding: 6px;");

        // Sets and reps
        workoutSetsInput = new QSpinBox(this);
        workoutSetsInput->setRange(1, 20);
        workoutSetsInput->setStyleSheet("background: rgba(255,255,255,0.1); color: #fff; border: 1px solid #fff; border-radius: 4px; padding: 6px;");

        workoutRepsInput = new QSpinBox(this);
        workoutRepsInput->setRange(1, 100);
        workoutRepsInput->setStyleSheet("background: rgba(255,255,255,0.1); color: #fff; border: 1px solid #fff; border-radius: 4px; padding: 6px;");
        QFormLayout* inputLayout = new QFormLayout(inputBox);
        inputLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
        inputLayout->setFormAlignment(Qt::AlignTop | Qt::AlignLeft);
        inputLayout->setSpacing(15);

        // Add form fields
        int labelWidth = 90; // Adjust as needed
        QLabel* nameLabel = new QLabel("Name:");
        nameLabel->setMinimumWidth(labelWidth);
        QLabel* targetLabel = new QLabel("Target:");
        targetLabel->setMinimumWidth(labelWidth);
        QLabel* descLabel = new QLabel("Description:");
        descLabel->setMinimumWidth(labelWidth);
        QLabel* setsLabel = new QLabel("Sets:");
        setsLabel->setMinimumWidth(labelWidth);
        QLabel* repsLabel = new QLabel("Reps:");
        repsLabel->setMinimumWidth(labelWidth);

        inputLayout->addRow(nameLabel, workoutNameInput);
        inputLayout->addRow(targetLabel, workoutTargetInput);
        inputLayout->addRow(descLabel, workoutDescInput);
        inputLayout->addRow(setsLabel, workoutSetsInput);
        inputLayout->addRow(repsLabel, workoutRepsInput);

        // Add/Delete buttons
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        QPushButton* addWorkoutButton = new QPushButton("Add Workout", this);
         deleteWorkoutButton = new QPushButton("Delete Workout", this);

        addWorkoutButton->setStyleSheet(
            "QPushButton {"
            "background: #00bfff;"
            "color: white;"
            "border: none;"
            "border-radius: 4px;"
            "padding: 8px;"
            "}"
            "QPushButton:hover { background: #0099cc; }"
            );

        deleteWorkoutButton->setStyleSheet(
            "QPushButton {"
            "background: #ff4444;"
            "color: white;"
            "border: none;"
            "border-radius: 4px;"
            "padding: 8px;"
            "}"
            "QPushButton:hover { background: #cc0000; }"
            );

        buttonLayout->addWidget(addWorkoutButton);
        buttonLayout->addWidget(deleteWorkoutButton);
        inputLayout->addRow("", buttonLayout);

        // --- Right: Workout Table ---
        QGroupBox* tableBox = new QGroupBox("", this);
        tableBox->setStyleSheet("color: #fff; background: rgba(0,0,0,0.2); border: 1px solid #fff; border-radius: 8px;");
        QVBoxLayout* tableLayout = new QVBoxLayout(tableBox);
        QLabel* workoutListTitle = new QLabel("Workout List", this);
        QFont workoutListFont = workoutListTitle->font();
        workoutListFont.setPointSize(16);
        workoutListFont.setBold(true);
        workoutListTitle->setFont(workoutListFont);
        workoutListTitle->setStyleSheet("color: #00bfff; margin-bottom: 8px;");
        QVBoxLayout* tableSectionLayout = new QVBoxLayout();
        tableSectionLayout->addWidget(workoutListTitle);
        tableSectionLayout->addWidget(tableBox);
        tableSectionLayout->addStretch();
        workoutTable = new QTableWidget(this);
        workoutTable->setColumnCount(5);
        workoutTable->setHorizontalHeaderLabels({"Name", "Target", "Description", "Sets", "Reps"});
        workoutTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        workoutTable->setStyleSheet(
            "QTableWidget {"
            "background: rgba(255,255,255,0.1);"
            "color: #fff;"
            "border: 1px solid #fff;"
            "border-radius: 4px;"
            "}"
            "QHeaderView::section {"
            "background: rgba(0,0,0,0.3);"
            "color: #fff;"
            "padding: 6px;"
            "border: none;"
            "}"
            "QTableWidget::item {"
            "padding: 6px;"
            "}"
            );

        tableLayout->addWidget(workoutTable);


        // Add layouts to main layout
        topLayout->addLayout(inputSectionLayout, 1);
        topLayout->addLayout(tableSectionLayout, 2);
        mainLayout->addLayout(topLayout);

        // Connect signals and slots
        connect(backFromWorkoutButton, &QPushButton::clicked, this, &MainWindow::onBackFromWorkoutButtonClicked);
        mainLayout->addWidget(backFromWorkoutButton, 0, Qt::AlignLeft);
        connect(addWorkoutButton, &QPushButton::clicked, this, &MainWindow::onAddWorkoutButtonClicked);
        connect(deleteWorkoutButton, &QPushButton::clicked, this, &MainWindow::onDeleteWorkoutButtonClicked);
        connect(workoutTable, &QTableWidget::itemSelectionChanged, this, [this]() {
            deleteWorkoutButton->setEnabled(workoutTable->currentRow() >= 0);
        });

        // Initial state
        deleteWorkoutButton->setEnabled(false);
        updateWorkoutTable();
    }


void MainWindow::setupCaloriesPage()
{
    caloriesPage = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(caloriesPage);
    QPushButton* backFromCaloriesButton = new QPushButton("← Back", this);
    backFromCaloriesButton->setFixedSize(90, 36);
    backFromCaloriesButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #222;"
        "  color: #00bfff;"
        "  border-radius: 8px;"
        "  font-weight: bold;"
        "  font-size: 16px;"
        "  padding: 4px 12px;"
        "  margin-bottom: 8px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #00bfff;"
        "  color: #fff;"
        "}"
        );
    mainLayout->addWidget(backFromCaloriesButton, 0, Qt::AlignLeft);

    // Set background gradient
   /* caloriesPage->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #232526, stop:1 #414345);"
        );*/

    // Title
    QLabel* bmrTitle = new QLabel("BMR Calculator", this);
    bmrTitle->setStyleSheet("color: #00bfff; font-size: 18px; font-weight: bold; margin-top: 16px;");

    QFormLayout* bmrForm = new QFormLayout();
    QDoubleSpinBox* bmrWeight = new QDoubleSpinBox(this);
    bmrWeight->setRange(30, 300);
    bmrWeight->setSuffix(" kg");
    bmrWeight->setValue(70);

    QDoubleSpinBox* bmrHeight = new QDoubleSpinBox(this);
    bmrHeight->setRange(100, 250);
    bmrHeight->setSuffix(" cm");
    bmrHeight->setValue(170);

    QSpinBox* bmrAge = new QSpinBox(this);
    bmrAge->setRange(10, 100);
    bmrAge->setValue(25);

    QComboBox* bmrGender = new QComboBox(this);
    bmrGender->addItems({"Male", "Female"});

    QPushButton* calcBmrBtn = new QPushButton("Calculate BMR", this);
    calcBmrBtn->setStyleSheet("background: #00bfff; color: #fff; border-radius: 8px; font-weight: bold;");

    QLabel* bmrResult = new QLabel("BMR: -- kcal/day", this);
    bmrResult->setStyleSheet("color: #fff; font-size: 15px; font-weight: bold;");

    bmrForm->addRow("Weight:", bmrWeight);
    bmrForm->addRow("Height:", bmrHeight);
    bmrForm->addRow("Age:", bmrAge);
    bmrForm->addRow("Gender:", bmrGender);
    bmrForm->addRow(calcBmrBtn);
    bmrForm->addRow(bmrResult);


    QLabel* titleLabel = new QLabel("Calorie & Macro Tracker", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #00bfff; margin-bottom: 24px;");
    mainLayout->addWidget(titleLabel);

    // Layouts
    QHBoxLayout* topLayout = new QHBoxLayout();
    QVBoxLayout* leftLayout = new QVBoxLayout();
    QVBoxLayout* centerLayout = new QVBoxLayout();
    QVBoxLayout* rightLayout = new QVBoxLayout();

    // --- Left: Food Search & Add ---
    QLabel* searchLabel = new QLabel("Search Food:", this);
    searchLabel->setStyleSheet("color: #fff;");
    QLineEdit* foodSearchInput = new QLineEdit(this);
    foodSearchInput->setPlaceholderText("Type food name...");
    foodSearchInput->setStyleSheet("background: rgba(0,0,0,0.2); color: #fff; border: 1px solid #fff; border-radius: 6px; padding: 6px; font-size: 15px;");
    QListWidget* foodSearchResults = new QListWidget(this);
    foodSearchResults->setStyleSheet("background: rgba(0,0,0,0.2); color: #fff; border-radius: 6px;");
    foodSearchResults->setObjectName("foodSearchResults");
    leftLayout->addWidget(searchLabel);
    leftLayout->addWidget(foodSearchInput);
    leftLayout->addWidget(foodSearchResults);

    // Custom food addition
    QGroupBox* customFoodBox = new QGroupBox("Add Custom Food", this);
    customFoodBox->setStyleSheet(
        "QGroupBox {"
        "  color: #fff;"
        "  background: transparent;"
        "  border: 1px solid #fff;"
        "  border-radius: 8px;"
        "  margin-top: 20px;" // Add space for the title
        "}"
        "QGroupBox::title {"
        "  subcontrol-origin: margin;"
        "  left: 10px;"
        "  top: 2px;"
        "  padding: 0 4px;"
        "  background: transparent;"
        "}"
        );
    QFormLayout* customFoodLayout = new QFormLayout(customFoodBox);
    customFoodLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    customFoodLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    customFoodLayout->setHorizontalSpacing(10);
    customFoodLayout->setVerticalSpacing(8);
    QLineEdit* customFoodName = new QLineEdit(this);
    QDoubleSpinBox* customFoodCalories = new QDoubleSpinBox(this);
    customFoodCalories->setRange(0, 2000);
    customFoodCalories->setSuffix(" kcal");
    QDoubleSpinBox* customFoodProtein = new QDoubleSpinBox(this);
    customFoodProtein->setRange(0, 200);
    customFoodProtein->setSuffix(" g");
    QDoubleSpinBox* customFoodCarbs = new QDoubleSpinBox(this);
    customFoodCarbs->setRange(0, 200);
    customFoodCarbs->setSuffix(" g");
    QDoubleSpinBox* customFoodFat = new QDoubleSpinBox(this);
    customFoodFat->setRange(0, 100);
    customFoodFat->setSuffix(" g");
    QPushButton* addCustomFoodButton = new QPushButton("Add Food", this);
    addCustomFoodButton->setStyleSheet("background: rgba(0,0,0,0.3); color: #fff; border-radius: 8px; border: 1px solid #fff;");
    customFoodLayout->addRow("Name:", customFoodName);
    customFoodLayout->addRow("Calories:", customFoodCalories);
    customFoodLayout->addRow("Protein:", customFoodProtein);
    customFoodLayout->addRow("Carbs:", customFoodCarbs);
    customFoodLayout->addRow("Fat:", customFoodFat);
    customFoodLayout->addRow(addCustomFoodButton);
    leftLayout->addWidget(customFoodBox);
    leftLayout->addStretch();

    // --- Center: Diary & Meal Selection ---
    QLabel* diaryTitle = new QLabel("Daily Food Diary", this);
    diaryTitle->setStyleSheet("color: #fff; font-size: 20px; font-weight: bold;");
    QDateEdit* diaryDateEdit = new QDateEdit(QDate::currentDate(), this);
    diaryDateEdit->setCalendarPopup(true);
    diaryDateEdit->setStyleSheet("background: rgba(0,0,0,0.2); color: #fff; border-radius: 6px;");
    diaryDateEdit->setObjectName("diaryDateEdit");
    QComboBox* mealCombo = new QComboBox(this);
    mealCombo->addItems({"Breakfast", "Lunch", "Dinner", "Snack"});
    mealCombo->setStyleSheet("background: rgba(0,0,0,0.2); color: #fff; border-radius: 6px;");
    mealCombo->setObjectName("mealCombo");
    QTableWidget* diaryTable = new QTableWidget(0, 5, this);
    diaryTable->setObjectName("diaryTable");
    QStringList diaryHeaders = {"Food", "Calories", "Protein", "Carbs", "Fat"};
    diaryTable->setHorizontalHeaderLabels(diaryHeaders);
    diaryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    diaryTable->setStyleSheet("background: rgba(0,0,0,0.2); color: #fff; border-radius: 6px;");
    QPushButton* removeFoodButton = new QPushButton("Remove Selected", this);
    removeFoodButton->setStyleSheet("background: rgba(0,0,0,0.3); color: #fff; border-radius: 8px; border: 1px solid #fff;");
    centerLayout->addWidget(diaryTitle);
    QHBoxLayout* dateMealLayout = new QHBoxLayout();
    dateMealLayout->addWidget(new QLabel("Date:", this));
    dateMealLayout->addWidget(diaryDateEdit);
    dateMealLayout->addWidget(new QLabel("Meal:", this));
    dateMealLayout->addWidget(mealCombo);
    centerLayout->addLayout(dateMealLayout);
    centerLayout->addWidget(diaryTable);
    centerLayout->addWidget(removeFoodButton);
    centerLayout->addStretch();

    // --- Right: Nutrition Summary, Goal, Progress, Macros ---
    QLabel* summaryTitle = new QLabel("Nutrition Summary", this);
    summaryTitle->setStyleSheet("color: #fff; font-size: 20px; font-weight: bold;");
    QLabel* caloriesLabel = new QLabel("Calories: 0 kcal", this);
    caloriesLabel->setObjectName("caloriesLabel");
    QLabel* proteinLabel = new QLabel("Protein: 0 g", this);
    proteinLabel->setObjectName("proteinLabel");
    QLabel* carbsLabel = new QLabel("Carbs: 0 g", this);
    carbsLabel->setObjectName("carbsLabel");
    QLabel* fatLabel = new QLabel("Fat: 0 g", this);
    fatLabel->setObjectName("fatLabel");
    caloriesLabel->setStyleSheet("color: #fff;");
    proteinLabel->setStyleSheet("color: #fff;");
    carbsLabel->setStyleSheet("color: #fff;");
    fatLabel->setStyleSheet("color: #fff;");
    rightLayout->addWidget(summaryTitle);
    rightLayout->addWidget(caloriesLabel);
    rightLayout->addWidget(proteinLabel);
    rightLayout->addWidget(carbsLabel);
    rightLayout->addWidget(fatLabel);

    QLabel* goalLabel = new QLabel("Daily Calorie Goal:", this);
    goalLabel->setStyleSheet("color: #fff;");
    QSpinBox* goalSpin = new QSpinBox(this);
    goalSpin->setObjectName("goalSpin");
    goalSpin->setRange(1000, 6000);
    goalSpin->setValue(2200);
    goalSpin->setSuffix(" kcal");
    QProgressBar* calorieProgress = new QProgressBar(this);
    calorieProgress->setObjectName("calorieProgress");
    calorieProgress->setRange(0, 6000);
    calorieProgress->setValue(0);
    calorieProgress->setStyleSheet("QProgressBar { background: #222; color: #fff; border-radius: 8px; } QProgressBar::chunk { background: #0078d7; }");
    rightLayout->addWidget(goalLabel);
    rightLayout->addWidget(goalSpin);
    rightLayout->addWidget(calorieProgress);
    rightLayout->addWidget(bmrTitle);
    rightLayout->addLayout(bmrForm);

    // Macro pie chart (Qt Charts)
    QChartView* macroChartView = nullptr;
#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    QPieSeries* macroSeries = new QPieSeries();
    macroSeries->append("Protein", 1);
    macroSeries->append("Carbs", 1);
    macroSeries->append("Fat", 1);
    QChart* macroChart = new QChart();
    macroChart->addSeries(macroSeries);
    macroChart->setTitle("Macros");
    macroChart->setBackgroundVisible(false);
    macroChartView = new QChartView(macroChart, this);
    macroChartView->setObjectName("macroChartView");
    macroChartView->setStyleSheet("background: transparent;");
    macroChartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(macroChartView);
#endif
    rightLayout->addStretch();

    // --- Add layouts to main ---
    topLayout->addLayout(leftLayout, 2);
    topLayout->addLayout(centerLayout, 4);
    topLayout->addLayout(rightLayout, 2);
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();

    // --- Connect signals and slots ---
    connect(backFromCaloriesButton, &QPushButton::clicked, this, &MainWindow::onBackFromCaloriesButtonClicked);
    connect(foodSearchInput, &QLineEdit::textChanged, this, &MainWindow::onFoodSearchTextChanged);
    connect(foodSearchResults, &QListWidget::itemClicked, this, &MainWindow::onFoodSelected);
    connect(diaryDateEdit, &QDateEdit::dateChanged, this, &MainWindow::updateNutritionSummary);
    connect(mealCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateNutritionSummary);
    connect(goalSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updateNutritionSummary);
    connect(removeFoodButton, &QPushButton::clicked, this, [this, diaryTable]() {
        int currentRow = diaryTable->currentRow();
        if (currentRow >= 0) {
            diaryTable->removeRow(currentRow);
            updateNutritionSummary();
        }
    });
   connect(addCustomFoodButton, &QPushButton::clicked, this, [this, customFoodName, customFoodCalories, customFoodProtein, customFoodCarbs, customFoodFat, foodSearchInput]() {
        if (customFoodName->text().isEmpty()) return;
        Food newFood;
        newFood.name = customFoodName->text();
        newFood.calories = customFoodCalories->value();
        newFood.protein = customFoodProtein->value();
        newFood.carbs = customFoodCarbs->value();
        newFood.fat = customFoodFat->value();
        foods.append(newFood);
        saveFoodsToFile();
        customFoodName->clear();
        customFoodCalories->setValue(0);
        customFoodProtein->setValue(0);
        customFoodCarbs->setValue(0);
        customFoodFat->setValue(0);
        onFoodSearchTextChanged(foodSearchInput->text());
    });
    connect(calcBmrBtn, &QPushButton::clicked, this, [=]() {
        double weight = bmrWeight->value();
        double height = bmrHeight->value();
        int age = bmrAge->value();
        bool isMale = (bmrGender->currentIndex() == 0);

        // Use your existing formula directly here
        double bmr = isMale
                         ? (10 * weight) + (6.25 * height) - (5 * age) + 5
                         : (10 * weight) + (6.25 * height) - (5 * age) - 161;

        bmrResult->setText(QString("BMR: %1 kcal/day").arg(qRound(bmr)));
    });
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        // Get current page
        QWidget* currentPage = stackedWidget->currentWidget();

        // If we're in weight tracking, calorie tracking, or workout page, go back to dashboard
        if (currentPage == weightTrackingPage ||
            currentPage == caloriesPage ||
            currentPage == workoutPage||
            currentPage == membersPage) {
            stackedWidget->setCurrentWidget(dashboardPage);
            return;
        }
    }

    // Call parent class implementation for other keys
    QMainWindow::keyPressEvent(event);
}

void MainWindow::onLoginButtonClicked()
{
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    // Check if username exists and password matches
    if (userCredentials.contains(username) && userCredentials[username] == password) {
        currentUser = username;
        loginStatusLabel->setText("Login successful!");
        // welcomeLabel->setText("Welcome to FitLife Gym Management System, " + username + "!");

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
    Member newMember;
    newMember.name = username; // Initially use username as name (can be updated later)
    newMember.email = email;
    newMember.username = username;
    newMember.password = password;
    newMember.membershipType = "Monthly"; // Default membership type
    newMember.joinDate = QDate::currentDate();
    newMember.expiryDate = newMember.joinDate.addMonths(1);
    members.append(newMember);

    saveMembersToFile(dataFilePath);

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
    qDebug() << "Logout clicked";
    currentUser = "";
    if (usernameInput) {
        qDebug() << "Clearing usernameInput";
        usernameInput->clear();
    } else {
        qDebug() << "usernameInput is nullptr";
    }
    if (passwordInput) {
        qDebug() << "Clearing passwordInput";
        passwordInput->clear();
    } else {
        qDebug() << "passwordInput is nullptr";
    }
    if (loginStatusLabel) {
        qDebug() << "Setting loginStatusLabel";
        loginStatusLabel->setText("Please log in to access the gym management system");
    } else {
        qDebug() << "loginStatusLabel is nullptr";
    }
    if (stackedWidget && loginPage) {
        qDebug() << "Switching to loginPage";
        stackedWidget->setCurrentWidget(loginPage);
    } else {
        qDebug() << "stackedWidget or loginPage is nullptr";
    }
}

void MainWindow::onMembersButtonClicked()
{

    bool ok;
    QString password = QInputDialog::getText(this, "Admin Access",
                                             "Enter admin password to access members page:", QLineEdit::Password,
                                             "", &ok);

    if (ok && !password.isEmpty()) {
        if (verifyAdminPassword(password)) {
            updateMembersTable();
            stackedWidget->setCurrentWidget(membersPage);
        } else {
            QMessageBox::warning(this, "Access Denied",
                                 "Invalid password. You don't have permission to access the members page.");
        }
    }
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
void MainWindow::onWeightTrackingButtonClicked()
{
    stackedWidget->setCurrentWidget(weightTrackingPage);
}
void MainWindow::onAddWeightButtonClicked()
{
    QDate date = weightDateEdit->date();
    double weight = weightInput->value();

    // Add to history
    weightHistory.append(qMakePair(date, weight));

    // Sort by date
    std::sort(weightHistory.begin(), weightHistory.end(),
              [](const QPair<QDate, double>& a, const QPair<QDate, double>& b) {
                  return a.first < b.first;
              });

    updateWeightTable();
    updateWeightChart();
    saveWeightHistoryToFile();

    QMessageBox::information(this, "Success", "Weight entry added successfully!");
}
void MainWindow::onDeleteWeightButtonClicked()
{
    int selectedRow = weightTable->currentRow();
    if (selectedRow >= 0 && selectedRow < weightHistory.size()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm Delete",
                                      "Are you sure you want to delete this weight entry?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            weightHistory.removeAt(selectedRow);
            updateWeightTable();
            updateWeightChart();
            saveWeightHistoryToFile();
            QMessageBox::information(this, "Success", "Weight entry deleted successfully!");
        }
    }
}
void MainWindow::onBackFromWeightButtonClicked()
{
    stackedWidget->setCurrentWidget(dashboardPage);
}

void MainWindow::updateMembersTable()
{
    // Check if membersTable is initialized
    if (!membersTable) {
        qDebug() << "Error: membersTable is not initialized";
        return;
    }

    // Clear the table
    membersTable->setRowCount(0);

    // Populate the table with members
    for (int i = 0; i < members.size(); ++i) {
        const Member& member = members[i];

        // Insert new row
        int row = membersTable->rowCount();
        membersTable->insertRow(row);

        // Create and set items with null checks
        QTableWidgetItem* nameItem = new QTableWidgetItem(member.name);
        QTableWidgetItem* phoneItem = new QTableWidgetItem(member.phone);
        QTableWidgetItem* emailItem = new QTableWidgetItem(member.email);
        QTableWidgetItem* typeItem = new QTableWidgetItem(member.membershipType);

        // Format dates safely
        QString joinDateStr = member.joinDate.isValid() ?
                                  member.joinDate.toString("dd/MM/yyyy") : "Invalid Date";
        QString expiryDateStr = member.expiryDate.isValid() ?
                                    member.expiryDate.toString("dd/MM/yyyy") : "Invalid Date";

        QTableWidgetItem* joinDateItem = new QTableWidgetItem(joinDateStr);
        QTableWidgetItem* expiryDateItem = new QTableWidgetItem(expiryDateStr);

        // Set items in table
        membersTable->setItem(row, 0, nameItem);
        membersTable->setItem(row, 1, phoneItem);
        membersTable->setItem(row, 2, emailItem);
        membersTable->setItem(row, 3, typeItem);
        membersTable->setItem(row, 4, joinDateItem);
        membersTable->setItem(row, 5, expiryDateItem);
    }

    // Resize columns to fit content
    membersTable->resizeColumnsToContents();
}
// ... existing code ...
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
//workout table update
void MainWindow::updateWeightTable()
{
    weightTable->setRowCount(0);
    for (int i = 0; i < weightHistory.size(); ++i) {
        weightTable->insertRow(i);
        weightTable->setItem(i, 0, new QTableWidgetItem(weightHistory[i].first.toString("dd/MM/yyyy")));
        weightTable->setItem(i, 1, new QTableWidgetItem(QString::number(weightHistory[i].second)));
    }
}
void MainWindow::updateWeightChart()
{

    QChart* chart = new QChart();
    chart->setTitle("Weight Progress");
    chart->setBackgroundVisible(false);
    chart->legend()->setVisible(false);

    QLineSeries* series = new QLineSeries();
    for (int i = 0; i < weightHistory.size(); ++i) {
        // Use QDateTime for the x value
        QDateTime dt(weightHistory[i].first, QTime(0, 0));
        series->append(dt.toMSecsSinceEpoch(), weightHistory[i].second);
    }

    chart->addSeries(series);

    QDateTimeAxis* axisX = new QDateTimeAxis();
    axisX->setFormat("dd/MM/yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("Weight (kg)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    weightChartView->setChart(chart);
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

    // Generate a username based on the name (remove spaces and make lowercase)
    QString username = name.toLower().replace(" ", "");

    // Check if username already exists, if so add a number
    QString baseUsername = username;
    int suffix = 1;
    while (userCredentials.contains(username)) {
        username = baseUsername + QString::number(suffix);
        suffix++;
    }

    // Generate a default password (could be improved for security)
    QString password = username + "123";

    Member newMember;
    newMember.name = name;
    newMember.phone = phone;
    newMember.email = email;
    newMember.username = username;
    newMember.password = password;
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

    // Add member to list and update credentials
    members.append(newMember);
    userCredentials[username] = password;

    updateMembersTable();

    // Clear inputs
    memberNameInput->clear();
    memberPhoneInput->clear();
    memberEmailInput->clear();

    QMessageBox::information(this, "Success", "Member added successfully!\n\nUsername: " + username + "\nPassword: " + password);
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
   QString target = workoutTargetInput->currentText();
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
   workoutTargetInput->setCurrentIndex(0);
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
