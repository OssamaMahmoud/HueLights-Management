#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WServer>
#include <Wt/WBootstrapTheme>

#include <Wt/Auth/AuthModel>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WFont>
#include <Wt/WImage>
#include <Wt/WString>
#include <Wt/WTable>
#include <Wt/WTableCell>

#include "Session.h"
#include "User.h"
#include "AuthWidget.h"
#include "Group.h"

//using namespace Wt;

class AuthApplication : public Wt::WApplication
{

public:
    AuthApplication(const Wt::WEnvironment& env)
            : Wt::WApplication(env),
              session_(appRoot() + "auth.db") {

        setTitle("Hue Lighting");
        Wt :: WApplication :: instance()-> useStyleSheet("css/styleSheet.css");

        toHome();



    }
    void toHome() {

        /* The orientation for table_ */
        Wt :: Orientation orientation1 = Wt :: Vertical;

        /*The orientation for tableInfo */
        Wt :: Orientation orientation2 = Wt :: Horizontal;

        /*Create Menu bar*/
        tableMenu = new Wt :: WTable(root());
        tableMenu-> setWidth(Wt :: WLength("100%"));
        tableMenu->setHeaderCount(10, orientation2);

        /* Create status flags on menu bar */
        statusHome = new Wt :: WText ("Home");
        statusHome-> setMargin(50, Wt :: Left);
        statusLogged = new Wt :: WText ("Not Logged In");
        statusLogged-> setMargin(1000, Wt :: Left);

        /* Add the status flags to the manu bar*/
        tableMenu->elementAt(0, 0)->addWidget(statusHome);
        tableMenu->elementAt(0, 10)->addWidget(statusLogged);

        /* Create the table for the header */
        table_ = new Wt :: WTable(root());
        table_-> setWidth(Wt :: WLength("100%"));
        table_->setHeaderCount(3, orientation1);

        Wt :: WText *pageHeader1 = new Wt :: WText("Welcome to");
        //pageHeader1-> setStyleClass("ruleHeader");

        Wt :: WText *pageHeader2 = new Wt :: WText("the Hue Lighting Control Center.");
        pageHeader2-> setStyleClass("rule2");

        /* Add the header elements to the table */
        table_->elementAt(0, 2)->addWidget(pageHeader1);
        table_->elementAt(1, 2)->addWidget(pageHeader2);
        table_->elementAt(0, 2)-> setStyleClass("ruleHeader");
        table_->elementAt(1, 2)-> setStyleClass("ruleHeader");

        /* The app description */
        Wt :: WText *description1 = new Wt :: WText(" The Hue Lighting Control System provides registered users with the options");
        Wt :: WText *description2 = new Wt :: WText(" to connect to individual or grouped lights and turn them on or off, change their colour,");
        Wt :: WText *description3 = new Wt :: WText(" and their brightness. Login or register below to begin.");

        /* Add the app description to a table */
        tableInfo = new Wt :: WTable(root());
        tableInfo-> setWidth(Wt :: WLength("100%"));
        tableInfo -> setHeaderCount(11, orientation2);

        tableInfo->elementAt(0, 6)->addWidget(description1);
        tableInfo->elementAt(1, 6)->addWidget(description2);
        tableInfo->elementAt(2, 6)->addWidget(description3);
        tableInfo->columnAt(5)->setStyleClass(".rule1");

        /* Create buttons, and add them to a button table */
        buttonLogin = new Wt :: WPushButton("Login", root());
        buttonLogin-> setMargin(230, Wt :: Right);

        buttonReg = new Wt :: WPushButton("Register", root());
        buttonLogin-> setMargin(230, Wt :: Right);

        tableButtons = new Wt :: WTable(root());
        tableButtons->setWidth(Wt :: WLength("100%"));
        tableButtons ->setHeaderCount(11, orientation2);

        tableButtons->elementAt(1, 5)->addWidget(buttonLogin);
        tableButtons->elementAt(1, 5)->addWidget(buttonReg);


        buttonLogin-> clicked().connect(this, &AuthApplication :: toLogin);
        buttonReg-> clicked().connect(this, &AuthApplication :: toRegister);

        /* If returning for register page, clear the register page elements and restore the home page title */
        if (fromRegisterPage == true) {
            registerPage-> clear();
            setTitle("Hue Lighting");
        }
    }


    void toLogin() {


        //group testing WORKS
        //the getstate does not actually get the actions for some reason..
        //but the result is visible in the actial emulator... wierd
        Bridge *bridge = new Bridge();
        bridge->defaultConnect("localhost", "8080","");
        Group *group = new Group();
        group->makeGroup("newdeveloper", "localhost", "8080", "LOOOOOOOOOOOOL", "\"1\"");
        std::cout << "\n\n\nthe grup id is:  " << group->getName() << " AND ID IS " << group->getId() <<endl;
        //sleep(2);
        group->setId("9");

        //group->getState("2");

        group->getGroups();

//        group->addLight("2");
        //group->getState("2");
        //group->changeState("false", "0", "30000", "255");
        //sleep(3);
        //group->getState("2");
        //      group->removeLight("1");
        //group->getState("2");
        // group->changeState("true", "255", "30000", "255");

        //group->deleteGroup();



        //std::cout << "\n\n\n\n current settings are "

        setTitle("Login");
        tableMenu -> clear();
        table_-> clear();
        tableInfo-> clear();
        tableButtons-> clear();

        /* If leaving the register page, clear the register page elements and replace title with the login page title */
        if (fromRegisterPage == true) {
            registerPage-> clear();
        }

        /* Create and use AuthWidget for authentication */
        session_.login().changed().connect(this, &AuthApplication::authEvent);

        root()->addStyleClass("container");
        setTheme(new Wt::WBootstrapTheme(this));


        AuthWidget *authWidget
                = new AuthWidget(session_);
        //authWidget->registrationModel();


        authWidget->model()->addPasswordAuth(&Session::passwordAuth());
        //authWidget->model()->addOAuth(Session::oAuth());
        authWidget->setRegistrationEnabled(true);

        authWidget->processEnvironment();

        root()->addWidget(authWidget);

    }

    void toRegister() {

        setTitle("Register");
        tableMenu -> clear();
        table_-> clear();
        tableInfo-> clear();
        tableButtons-> clear();

        /* Create a table to add the home and register buttons to */
        registerPage = new Wt :: WTable(root());
        registerPage-> setWidth(Wt :: WLength("65%"));

        registerButton =  new Wt :: WPushButton("Register Now");

        registerPage-> elementAt(0, 0)->addWidget(registerButton);

        registerButton-> clicked().connect(this, &AuthApplication :: toLogin);
        fromRegisterPage = true;

        homeButton =  new Wt :: WPushButton("Back to Home");

        registerPage-> elementAt(0, 0)->addWidget(homeButton);
        homeButton-> clicked().connect(this, &AuthApplication :: toHome);

    }

    /* Authentication event which initalizes new user session */
    void authEvent() {
        if (session_.login().loggedIn()) {
            const Wt::Auth::User& u = session_.login().user();
            //redirect to bridge page here

            Wt::Dbo::Transaction t(session_);
            dbo::ptr<User> user = session_.user();
            //statusLogged->setText("Logged in as:" + user->getFName() + user->getLName());

            Wt::log("\nNOTICE")
                    << "firstname " << user->getFName() << " lastname " << user->getLName();

        } else{
            Wt::log("notice") << "User logged out.";

        }

    }

private:
    Session session_;

    Wt :: WText *statusHome, *statusLogged;
    Wt :: WPushButton *buttonLogin, *buttonReg, *homeButton, *registerButton;
    Wt :: WTable *tableMenu, *table_, *tableInfo, *tableButtons, *registerPage;

    bool fromRegisterPage = false;

};

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    AuthApplication *app = new AuthApplication(env);
    app->messageResourceBundle().use("templates");
    app->messageResourceBundle().use(AuthApplication::appRoot() + "templates");

    return app;
}



int main(int argc, char **argv)
{

    try {
        Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);

        server.addEntryPoint(Wt::Application, createApplication);

        Session::configureAuth();

        server.run();
    } catch (Wt::WServer::Exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }
}