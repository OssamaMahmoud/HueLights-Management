
 //Created by ubuntu on 11/3/17.


#ifndef AUTH_MYAUTHWIDGET_H
#define AUTH_MYAUTHWIDGET_H

#include <string.h>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/Dbo/UserDatabase>
#include <Wt/WBootstrapTheme>
#include <Wt/WDialog>
#include <Wt/WTable>
#include <Wt/WMessageBox>
#include "Bridge.h"
#include "Group.h"
#include "RegistrationView.h"
#include "Session.h"
#include "User.h"

 class Session;

 class AuthWidget : public Wt::Auth::AuthWidget
 {
     public:
         AuthWidget(Session& session);

         /* We will use a custom registration view */
         virtual Wt::WWidget *createRegistrationView(const Wt::Auth::Identity& id);

     private:
         Session& session_;

         Bridge bridge_;

         Wt :: WLineEdit *bridgeAddress_, *bridgePort_, *bridgeReference_ ;
         Wt :: WTable *choosePage,*table_;
         Wt :: WPushButton  *buttonChoose;
         string address, port, reference;

         Group *group;
         Wt::WDialog *addDialog_,*modDialog_,*delDialog_;
         WTable *displayTable;
         WLineEdit *addName_,*addLights,*delGroupID,*id,*on,*bri,*hue,*sat,*modLights;

         void createLoggedInView();
         void MainPage();
         void ConnectToBridge();
         void lightPage();
         void schedulePage();
         void groupPage();
         void showGroupAdd();
         void addDialogDone(Wt::WDialog::DialogCode code);
         void showGroupModify();
         void modDialogDone(Wt::WDialog::DialogCode code);
         void showGroupDelete();
         void delDialogDone(Wt::WDialog::DialogCode code);
         void viewNow();
         void individualGroupButton(string id);
         void getGroupsIdHandler();
 };



#endif //AUTH_MYAUTHWIDGET_H
