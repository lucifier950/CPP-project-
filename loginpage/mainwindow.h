#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QStackedWidget>
#include <QMap>
#include <QString>
#include <QTableWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTabWidget>
#include <QDateTime>
#include <QListWidget>
#include <QGroupBox>
#include <QFormLayout>
#include <QTextEdit>
#include <QHeaderView>
#include <QDateEdit>



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // file handling functions - now accessible everywhere
    void saveMembersToFile(const QString& filePath);
    void loadMembersFromFile(const QString& filePath);

private slots:
    // Login page slots
    void onLoginButtonClicked();
    void onClearLoginButtonClicked();
    void onShowSignUpClicked();

    // Sign Up page slots
    void onSignUpButtonClicked();
    void onClearSignUpButtonClicked();
    void onShowLoginClicked();

    // Dashboard slots
    void onLogoutButtonClicked();
    void onCalorieCalculatorButtonClicked();
    void onMembersButtonClicked();
    void onWorkoutPlanButtonClicked();

    // Members Management slots
    void onAddMemberButtonClicked();
    void onDeleteMemberButtonClicked();
    void onEditMemberButtonClicked();
    void onBackToMainButtonClicked();

    // Workout Plan slots
    void onAddWorkoutButtonClicked();
    void onDeleteWorkoutButtonClicked();
    void onBackFromWorkoutButtonClicked();

    // Calorie Calculator slots
    void onCalculateCaloriesButtonClicked();
    void onGenderSelectionChanged();
    void onActivityLevelChanged(int index);
    void onBackFromCaloriesButtonClicked();

private:
    // User data storage (in a real app, this would be in a database)
    QMap<QString, QString> userCredentials;
    QString currentUser;
    QString dataFilePath;

    class Member {
    public:
        QString name;
        QString phone;
        QString email;
        QString username;
        QString password;
        QString membershipType;
        QDate joinDate;
        QDate expiryDate;
    };

    class Workout {
    public:
        QString name;
        QString target;
        QString description;
        int sets;
        int reps;
    };
    class Food {
    public:
        QString name;
        double calories;
        double protein;  // in grams
        double carbs;    // in grams
        double fat;      // in grams
    };

    QList<Member> members;
    QList<Workout> workouts;
    QVector<Food> foods;  // Add this to store your food database
    QMap<QDate, QMap<QString, QVector<Food>>> foodDiary;

    // Main widget and stacked layout for switching between pages
    QWidget *centralWidget;
    QStackedWidget *stackedWidget;

    // Login page widgets
    QWidget *loginPage;
    QGridLayout *loginLayout;
    QLabel *loginTitleLabel;
    QLabel *usernameLabel;
    QLineEdit *usernameInput;
    QLabel *passwordLabel;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QPushButton *clearLoginButton;
    QPushButton *showSignUpButton;
    QLabel *loginStatusLabel;

    // Sign Up page widgets
    QWidget *signUpPage;
    QGridLayout *signUpLayout;
    QLabel *signUpTitleLabel;
    QLabel *newUsernameLabel;
    QLineEdit *newUsernameInput;
    QLabel *newPasswordLabel;
    QLineEdit *newPasswordInput;
    QLabel *confirmPasswordLabel;
    QLineEdit *confirmPasswordInput;
    QLabel *emailLabel;
    QLineEdit *emailInput;
    QPushButton *signUpButton;
    QPushButton *clearSignUpButton;
    QPushButton *showLoginButton;
    QLabel *signUpStatusLabel;

    // Dashboard widgets
    QWidget *dashboardPage;
    QVBoxLayout *dashboardLayout;
    QLabel *welcomeLabel;
    QPushButton *membersButton;
    QPushButton *workoutPlanButton;
    QPushButton *calorieCalculatorButton;
    QPushButton *logoutButton;

    // Members Management widgets
    QWidget *membersPage;
    QVBoxLayout *membersLayout;
    QLabel *membersTitle;
    QTableWidget *membersTable;
    QHBoxLayout *memberButtonsLayout;
    QPushButton *addMemberButton;
    QPushButton *editMemberButton;
    QPushButton *deleteMemberButton;
    QPushButton *backToMainButton;
    QLineEdit *memberNameInput;
    QLineEdit *memberPhoneInput;
    QLineEdit *memberEmailInput;
    QComboBox *membershipTypeCombo;

    // Workout Plan widgets
    QWidget *workoutPage;
    QVBoxLayout *workoutLayout;
    QLabel *workoutTitle;
    QTableWidget *workoutTable;
    QHBoxLayout *workoutButtonsLayout;
    QPushButton *addWorkoutButton;
    QPushButton *deleteWorkoutButton;
    QPushButton *backFromWorkoutButton;
    QLineEdit *workoutNameInput;
    QLineEdit *workoutTargetInput;
    QTextEdit *workoutDescInput;
    QSpinBox *workoutSetsInput;
    QSpinBox *workoutRepsInput;

    // Calorie Calculator widgets
    QWidget *caloriesPage;
    QVBoxLayout *caloriesLayout;
    QLabel *caloriesTitle;

    QGroupBox *personalInfoGroup;
    QFormLayout *personalInfoLayout;
    QSpinBox *ageInput;
    QRadioButton *maleRadio;
    QRadioButton *femaleRadio;
    QButtonGroup *genderGroup;
    QDoubleSpinBox *weightInput;
    QDoubleSpinBox *heightInput;

    QGroupBox *activityGroup;
    QFormLayout *activityLayout;
    QComboBox *activityLevelCombo;
    QLabel *activityDescriptionLabel;

    QGroupBox *resultsGroup;
    QFormLayout *resultsLayout;
    QLabel *bmrResultLabel;
    QLabel *tdeeResultLabel;
    QLabel *weightLossLabel;
    QLabel *weightGainLabel;
    QLabel *maintainWeightLabel;

    QPushButton *calculateCaloriesButton;
    QPushButton *backFromCaloriesButton;

    // Methods to create UI components
    void setupLoginPage();
    void setupSignUpPage();
    void setupDashboardPage();
    void setupMembersPage();
    void setupWorkoutPage();
    void setupCaloriesPage();

    // Helper methods
    void updateMembersTable();
    void updateWorkoutTable();
    void loadSampleData();
    double calculateBMR();
    double calculateTDEE(double bmr);
};

#endif // MAINWINDOW_H
