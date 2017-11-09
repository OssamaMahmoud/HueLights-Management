#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WServer>
#include <Wt/WBootstrapTheme>

#include <Wt/Auth/AuthModel>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>

#include "Session.h"
#include "User.h"
#include "AuthWidget.h"

class AuthApplication : public Wt::WApplication
{



public:
  AuthApplication(const Wt::WEnvironment& env)
    : Wt::WApplication(env),
      session_(appRoot() + "auth.db")
  {
    session_.login().changed().connect(this, &AuthApplication::authEvent);

    root()->addStyleClass("container");
    setTheme(new Wt::WBootstrapTheme(this));

    //useStyleSheet("css/style.css");


    AuthWidget *authWidget
      = new AuthWidget(session_);
    //authWidget->registrationModel();


    authWidget->model()->addPasswordAuth(&Session::passwordAuth());
    //authWidget->model()->addOAuth(Session::oAuth());
    authWidget->setRegistrationEnabled(true);

    authWidget->processEnvironment();

    root()->addWidget(authWidget);
  }

    //todo shit, can do anything here
  void authEvent() {
    if (session_.login().loggedIn()) {
      const Wt::Auth::User& u = session_.login().user();
      Wt::log("notice")
	<< "User " << u.id()
	<< " (" << u.identity(Wt::Auth::Identity::LoginName) << ")"
	<< " logged in.";
    } else{
      Wt::log("notice") << "User logged out.";

    }

  }

private:
  Session session_;
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