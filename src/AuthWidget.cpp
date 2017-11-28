#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "AuthWidget.h"

AuthWidget::AuthWidget(Session& session): Wt::Auth::AuthWidget(Session::auth(), session.users(),
                                                               session.login()),session_(session){
    group = new Group();
}


Wt::WWidget *AuthWidget::createRegistrationView(const Wt::Auth::Identity& id){
    RegistrationView *w = new RegistrationView(session_, this);
    Wt::Auth::RegistrationModel *model = createRegistrationModel();
    if (id.isValid())
        model->registerIdentified(id);
    w->setModel(model);
    return w;
}


void AuthWidget::createLoggedInView(){
    setTemplateText(tr("template.loggedin"));
    address ="";
    port= "";

    Wt::Dbo::Transaction t(session_);
    dbo::ptr<User> user = session_.user();

    //this is where u can add a widget
    WPushButton *logout = new WPushButton("LOGOUT");
    WText *name = new WText("Hello "  + user->getFName() + "!");


    Wt :: Orientation orientation1 = Wt :: Vertical;
    //Wt :: Orientation orientation2 = Wt :: Horizontal;

    table_ = new Wt :: WTable();
    table_-> setWidth(Wt :: WLength("100%"));
    table_->setHeaderCount(3, orientation1);

    WText *pageHeader = new WText("This is the bridge view page");
    pageHeader-> setStyleClass("rule2");

    table_->elementAt(1, 2)->addWidget(pageHeader);
    choosePage = new Wt :: WTable();
    choosePage-> setWidth(Wt :: WLength("85%"));
    //choosePage-> setHeaderCount(11, orientation2);

    Wt :: WText *description1 = new Wt :: WText("The url you want to connect is:");
    choosePage->elementAt(0,0)->addWidget(description1);


    Wt :: WText *description2 = new Wt :: WText("Address: ");
    bridgeAddress_ = new Wt:: WLineEdit();
    bridgeAddress_->setInputMask("009.009.009.009;_");
    bridgeAddress_->setPlaceholderText("enter address");
    choosePage->elementAt(1,0)->addWidget(description2);
    choosePage->elementAt(1,1)->addWidget(bridgeAddress_);

    Wt :: WText *description3 = new Wt :: WText("Port: ");
    bridgePort_ = new Wt:: WLineEdit();
    bridgePort_->setInputMask("9999;_");
    bridgePort_->setPlaceholderText("enter port");
    choosePage->elementAt(2,0)->addWidget(description3);
    choosePage->elementAt(2,1)->addWidget(bridgePort_);

    Wt :: WText *description4 = new Wt :: WText("Reference:");
    bridgeReference_ = new Wt:: WLineEdit();
    bridgeReference_->setPlaceholderText("enter reference");
    choosePage->elementAt(3,0)->addWidget(description4);
    choosePage->elementAt(3,1)->addWidget(bridgeReference_);

    buttonChoose = new Wt :: WPushButton("Confirm");
    choosePage->elementAt(4,0)->addWidget(buttonChoose);
    buttonChoose-> clicked().connect(this,  &AuthWidget::ConnectToBridge);

    //connecting the logout button
    logout->clicked().connect(&login(), &Auth::Login::logout);

    //bind to name put in the template
    bindWidget("name", name);
    bindWidget("logoutButton", logout);
    bindWidget("header", pageHeader);
    bindWidget("table",choosePage);

    //if from here we got the ip addresses of bridge and port
    /*
     * Jimmy- I'm waiting to see where this should go, it has a bad side effect right now
    user.modify()->setBridgePort("8080");
    user.modify()->setBridgeIp("localhost");


    session_.flush();
    */



}

// Check to see if bridge information is correct
void AuthWidget::ConnectToBridge() {
    //saving the input from the user as a c-string
    address = bridgeAddress_->text().toUTF8();
    port = bridgePort_->text().toUTF8();
    reference = bridgeReference_->text().toUTF8();

    //setting the group's reference to the emulator
    group->setPort(port);
    group->setAddress(address);
    group->setUsername("newdeveloper");

    //connecting to the bridge
    bridge_.defaultConnect(address,port,reference);

    //if the url is correct, redirect
    if (bridge_.get_success().compare("correct url") ==0){
        MainPage();
    }
    else{
        //display to the user
       Wt::WMessageBox::show("Incorrect Bridge Information",
                                        "<p>Please make sure your IP and Port are correct!</p>",Wt::StandardButton::Ok);
    }
}

void AuthWidget::MainPage(){

    setTemplateText(tr("template.loggedinAfterMain"));
    Wt::Dbo::Transaction t(session_);
    dbo::ptr<User> user = session_.user();

    WPushButton *back = new Wt :: WPushButton("Back");
    back->clicked().connect(this,&AuthWidget::createLoggedInView);

    //creating the header for the page
    Wt :: WText *prompt = new Wt :: WText("Navigation");
    WPushButton *lights = new Wt :: WPushButton("Lights");
    WPushButton *groups = new Wt :: WPushButton("Groups");
    WPushButton *schedules = new Wt :: WPushButton("Schedules");

    //creating the table to add to the page
    table_ = new Wt :: WTable();
    table_-> setWidth(Wt :: WLength("100%"));
    table_->elementAt(3,3)->addWidget(lights);
    table_->elementAt(3,4)->addWidget(groups);
    table_->elementAt(3,5)->addWidget(schedules);
    lights->clicked().connect(this, &AuthWidget::lightPage);
    groups->clicked().connect(this, &AuthWidget::groupPage);
    schedules->clicked().connect(this, &AuthWidget::schedulePage);
    //adding to the page
    bindWidget("back",back);
    bindWidget("header", prompt);
    bindWidget("table", table_);
}
/*
 *  IMPORTANT!!!!!!!!!!!!!
 *  MAKE SURE YOUR CMAKE TEMPLATE HAS THE message id=template.logginednAfterMain view
 *
 */

void AuthWidget:: lightPage(){
    //yumeng
    setTemplateText(tr("template.loggedinAfterMain"));
    Wt::Dbo::Transaction t(session_);
    dbo::ptr<User> user = session_.user();
    WPushButton *back = new Wt :: WPushButton("Back");
    back->clicked().connect(this,&AuthWidget::MainPage);
    /*
    * PROMPT WILL BE THE FIRST LINE AFTER LOGOUT
    * TABLE IS EVERYTHING THAT COMES AFTER

          add code here
       bindWidget("back",back);
       bindWidget("header", prompt);
       bindWidget("table", table_);
    */
}

void AuthWidget:: schedulePage(){
    //marlin
    setTemplateText(tr("template.loggedinAfterMain"));
    Wt::Dbo::Transaction t(session_);
    dbo::ptr<User> user = session_.user();
    WPushButton *back = new Wt :: WPushButton("Back");
    back->clicked().connect(this,&AuthWidget::MainPage);
    /*
     * PROMPT WILL BE THE FIRST LINE AFTER LOGOUT
     * TABLE IS EVERYTHING THAT COMES AFTER

           add code here
        bindWidget("back",back);
        bindWidget("header", prompt);
        bindWidget("table", table_);
     */
}

void AuthWidget::groupPage(){
    setTemplateText(tr("template.loggedinAfterMain"));
    Wt::Dbo::Transaction t(session_);
    dbo::ptr<User> user = session_.user();

    WPushButton *back = new Wt :: WPushButton("Back");
    back->clicked().connect(this,&AuthWidget::MainPage);

    table_ = new Wt :: WTable();
    Wt :: WText *prompt = new Wt :: WText("                 GROUPS                   ");
    WPushButton *view = new Wt :: WPushButton("View");
    WPushButton *add = new Wt :: WPushButton("Add");
    WPushButton *modify = new Wt :: WPushButton("Modify");
    WPushButton *del = new Wt :: WPushButton("Delete");
    //creating the table to add to the page
    WTable *table_ = new Wt :: WTable();
    table_->elementAt(3,3)->addWidget(view);
    table_->elementAt(3,4)->addWidget(add);
    table_->elementAt(3,5)->addWidget(modify);
    table_->elementAt(3,6)->addWidget(del);
    add->clicked().connect(this, &AuthWidget::showGroupAdd);
    modify->clicked().connect(this, &AuthWidget::showGroupModify);
    del->clicked().connect(this, &AuthWidget::showGroupDelete);
    bindWidget("back",back);
    bindWidget("header", prompt);
    bindWidget("table", table_);

    /*
    user.modify()->setBridgePort("8080");
    user.modify()->setBridgeIp("localhost");



    group->setPort(user->getBridgePort());
    group->setAddress(user->getBridgeIp());

    group->getGroups();


    string IDs = group->getGroupIdList();//will be empty

    WPushButton *pushNow = new WPushButton("Push twice");

    pushNow->clicked().connect(this, &AuthWidget::pushNowHandler);

    bindWidget("pushNow", pushNow);

    WText *result = new WText("Result");
    bindWidget("result", result);
*/


}
void AuthWidget::pushNowHandler(){
    string ID = group->getGroupIdList();
    string strRes = "";
    cout << "the id is:       " << ID << endl;
    vector<string> idVector;
    boost::split(idVector, ID, boost::is_any_of(","));
    for (int i = 0; i < idVector.size(); i++) {

        strRes.append("\nGroup ID: " + idVector[i]);

    }
}


void AuthWidget::showGroupAdd() {
    //setting the title
    addDialog_ = new WDialog("Add");

    //creating the name text field
    new WText("Name:",addDialog_->contents());
    addName_ = new Wt::WLineEdit(addDialog_->contents());
    new WText("<br />",addDialog_->contents());

    //creating the lights text field
    new WText("Lights:",addDialog_->contents());
    addLights = new Wt::WLineEdit(addDialog_->contents());
    new WText("<br />",addDialog_->contents());

    //creating the ok button
    Wt::WPushButton *ok = new Wt::WPushButton("Ok", addDialog_->contents());
    addName_->enterPressed().connect(addDialog_,&Wt::WDialog::accept);
    addLights->enterPressed().connect(addDialog_,&Wt::WDialog::accept);
    ok->clicked().connect(addDialog_,&Wt::WDialog::accept);

    //displaying
    addDialog_->finished().connect(this, &AuthWidget::addDialogDone);
    addDialog_->show();

}
void AuthWidget::addDialogDone(Wt::WDialog::DialogCode code){
    if(code == Wt::WDialog::Accepted){

        cout<<("Welcome, "+addName_->text())<<endl;
        //group->makeGroup("newdeveloper", address,port,)

        //if makeGroup returns true
        if (addName_->text().toUTF8().c_str() != "" && addLights->text().toUTF8().c_str()!="") {
            Wt::WMessageBox::show("Success!",
                                  "<p>You have successfully added a group.</p>", Wt::StandardButton::Ok);
        }
    }
    delete addDialog_;
}


void AuthWidget::showGroupModify() {
    //WDialog modDialog_ = new WDialog("Modify");

}

void AuthWidget::modDialogDone(Wt::WDialog::DialogCode code){
    if(code == Wt::WDialog::Accepted){

        Wt::WMessageBox::show("Success!",
                              "<p>You have successfully modified a group.</p>",Wt::StandardButton::Ok);
    }
    delete modDialog_;
}

void AuthWidget::showGroupDelete() {
    delDialog_ = new WDialog("Delete");

    new WText("Group Id:",delDialog_->contents());
    delGroupID = new Wt::WLineEdit(delDialog_->contents());
    delGroupID->setInputMask("09;_");
    new WText("<br />",delDialog_->contents());

    Wt::WPushButton *ok = new Wt::WPushButton("Ok", delDialog_->contents());
    delGroupID->enterPressed().connect(delDialog_,&Wt::WDialog::accept);
    ok->clicked().connect(delDialog_,&Wt::WDialog::accept);

    delDialog_->finished().connect(this, &AuthWidget::delDialogDone);
    delDialog_->show();

}

void AuthWidget::delDialogDone(Wt::WDialog::DialogCode code){
    if(code == Wt::WDialog::Accepted){
        int i;
        i =group->deleteGroup(delGroupID->text().toUTF8());
        cout<<"i = "+ i <<endl;

        //if delete group returns 0;
        Wt::WMessageBox::show("Success!",
                              "<p>You have successfully deleted group " + delGroupID->text().toUTF8()+".</p>",Wt::StandardButton::Ok);
    }
    delete delDialog_;
}