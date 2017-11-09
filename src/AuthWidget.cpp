#include "AuthWidget.h"
#include "RegistrationView.h"
#include "Session.h"

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