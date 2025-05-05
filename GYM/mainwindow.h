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
#include <QProgressBar>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QPixmap>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void saveMembersToFile(const QString& filePath);
    void loadMembersFromFile(const QString& filePath);
    void saveWeightHistoryToFile();
    void loadWeightHistoryFromFile();
protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    // Login page slots
    void onLoginButtonClicked();
    void onClearLoginButtonClicked();
    void onShowSignUpClicked();
    //weight tracking
    void setupWeightTrackingPage();
    void onWeightTrackingButtonClicked();
    void onAddWeightButtonClicked();
    void onDeleteWeightButtonClicked();
    void onBackFromWeightButtonClicked();
    void updateWeightTable();
    void updateWeightChart();

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

    // Calorie Tracking slots
    void onFoodSearchTextChanged(const QString &text);
    void onFoodSelected(QListWidgetItem* item);
    void updateNutritionSummary();
    void updateMacroChart(double protein, double carbs, double fat);

private:
    // User data storage
    QMap<QString, QString> userCredentials;
    QString currentUser;
    QString dataFilePath;
    QString weightHistoryFilePath;
    //this the password i have added to give access to add member page:
    const QString ADMIN_PASSWORD = "admin123";
    bool verifyAdminPassword(const QString& password) {
        return password == ADMIN_PASSWORD;
    }


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
        double protein;
        double carbs;
        double fat;
    };

    QList<Member> members;
    QList<Workout> workouts;
    QVector<Food> foods;
    QMap<QDate, QMap<QString, QVector<Food>>> foodDiary;

    // Calorie tracking file paths
    QString foodsFilePath;
    QString foodDiaryFilePath;

    // Calorie tracking file handling
    void saveFoodsToFile();
    void loadFoodsFromFile();
    void saveFoodDiaryToFile();
    void loadFoodDiaryFromFile();

    // Main widget and stacked layout for switching between pages
    QWidget *centralWidget;
    QStackedWidget *stackedWidget;
    //weight tracking widget
    QWidget* weightTrackingPage;
    QTableWidget* weightTable;
    QDateEdit* weightDateEdit;
    QPushButton* addWeightButton;
    QPushButton* deleteWeightButton;
    QPushButton* backFromWeightButton;
    QChartView* weightChartView;
    QVector<QPair<QDate, double>> weightHistory;

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
    QComboBox* workoutTargetInput;
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
