#include "AuthWidget.h"
#include "RegistrationView.h"
#include "Session.h"
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WServer>
#include <Wt/WBootstrapTheme>

#include <Wt/Auth/AuthModel>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>


AuthWidget::AuthWidget(Session& session)
        : Wt::Auth::AuthWidget(Session::auth(), session.users(), session.login()),
          session_(session)
{  }


Wt::WWidget *AuthWidget::createRegistrationView(const Wt::Auth::Identity& id)
{
    RegistrationView *w = new RegistrationView(session_, this);
    Wt::Auth::RegistrationModel *model = createRegistrationModel();

    if (id.isValid())
        model->registerIdentified(id);

    w->setModel(model);
    return w;
}


void AuthWidget::createLoggedInView(){

    setTemplateText(tr("template.loggedin"));

    //Wt::Auth::AuthWidget::createLoggedInView();
    Wt::Dbo::Transaction t(session_);
    dbo::ptr<User> user = session_.user();

    //this is where u can add a widget
    WPushButton *logout = new WPushButton("LOGOUT");
    WText *name = new WText("First Name: "  + user->getFName() + " Last Name: " + user->getLName() );
    //Bridge *bridge =  new Bridge();
    //bridge->defaultConnect("0.0.0.0", "8080", "LOOL");

    //WPushButton *bridge = new WPushButton("BRIDGE");


    //printf("\n\n\n\n\n\n email    %s", user->getAuthInfo()->email());
    //button tings
    logout->clicked().connect(&login(), &Auth::Login::logout);

    //bind to name put in the template
    bindWidget("name", name);
    bindWidget("logoutButton", logout);
    //bindWidget("bridge", bridge);

    //

}