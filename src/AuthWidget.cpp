#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>
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
    if(user->getBridgeIp().compare("")!=0){
        bridgeAddress_->setText((user->getBridgeIp()));
    }
    choosePage->elementAt(1,0)->addWidget(description2);
    choosePage->elementAt(1,1)->addWidget(bridgeAddress_);

    Wt :: WText *description3 = new Wt :: WText("Port: ");
    bridgePort_ = new Wt:: WLineEdit();
    bridgePort_->setInputMask("9999;_");
    bridgePort_->setPlaceholderText("enter port");
    if(user->getBridgePort().compare("")!=0){
        bridgePort_->setText((user->getBridgePort()));
    }
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


}

// Check to see if bridge information is correct
void AuthWidget::ConnectToBridge() {
    Wt::Dbo::Transaction t(session_);
    dbo::ptr<User> user = session_.user();
    //saving the input from the user as a c-string

    address = bridgeAddress_->text().toUTF8();
    port = bridgePort_->text().toUTF8();
    reference = bridgeReference_->text().toUTF8();

    user.modify()->setBridgeIp(address);
    user.modify()->setBridgePort(port);
    session_.flush();


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

    WPushButton *back = new Wt :: WPushButton("Back To Bridge");
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
    //connect the buttons to the pages
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
    WPushButton *view = new Wt :: WPushButton("UpdateView");
    WPushButton *add = new Wt :: WPushButton("Add");
    WPushButton *modify = new Wt :: WPushButton("Modify");
    WPushButton *del = new Wt :: WPushButton("Delete");

    //need this for the get views
    WPushButton *getGroupsId = new Wt :: WPushButton("View Groups");





    //creating the table to add to the page
    table_ = new Wt :: WTable();
    table_->elementAt(3,3)->addWidget(view);
    table_->elementAt(3,4)->addWidget(add);
    table_->elementAt(3,5)->addWidget(modify);
    table_->elementAt(3,6)->addWidget(del);

    table_->elementAt(4,3)->addWidget(getGroupsId);

    innerTable = new Wt :: WTable();
    table_->elementAt(5,5)->addWidget(innerTable);


    add->clicked().connect(this, &AuthWidget::showGroupAdd);
    modify->clicked().connect(this, &AuthWidget::showGroupModify);
    del->clicked().connect(this, &AuthWidget::showGroupDelete);
    bindWidget("back",back);
    bindWidget("header", prompt);
    bindWidget("table", table_);



    group->setPort(user->getBridgePort());
    group->setAddress(user->getBridgeIp());


    group->getGroups();


    string IDs = group->getGroupIdList();//will be empty


   //getGroupsId->clicked().connect(this, &AuthWidget::getGroupsIdHandler);

    view->clicked().connect(this,&AuthWidget::viewNow);

}


void AuthWidget::viewNow(){
}

void AuthWidget::individualGroupButton(string id){
    group->getState(id);
}

void AuthWidget::getGroupsIdHandler() {
    string ID = group->getGroupIdList();
    cout << "the id is:       " << ID << endl;
    vector<string> idVector;
    innerTable->clear();
    boost::split(idVector, ID, boost::is_any_of(","));
    for (int i = 0; i < idVector.size() && !idVector.size() == 0; i++) {
        WPushButton *pushButton = new WPushButton("Get Info group: " + idVector[i]);
        boost::trim(idVector[i]);
        pushButton->clicked().connect( boost::bind(&AuthWidget::individualGroupButton, this, idVector[i] ) );
        //table_->hide();
        this->innerTable->elementAt(0, i)->addWidget(pushButton);

    }
}


void AuthWidget::showGroupAdd() {
    //setting the title
    addDialog_ = new WDialog("Add");

    //creating the name text field
    new WText("Name:",addDialog_->contents());
    addName_ = new Wt::WLineEdit(addDialog_->contents());
    new WText("<br />",addDialog_->contents());
    new WText("Have your lights in this format: \"1\",\"2\" ",addDialog_->contents());
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
    addDialog_->setMovable(false);

}
void AuthWidget::addDialogDone(Wt::WDialog::DialogCode code){
    if(code == Wt::WDialog::Accepted){
        string name,lights;
        name = addName_->text().toUTF8();
        lights = addLights->text().toUTF8();
        cout<<("Welcome, "+addName_->text())<<endl;

        //if makeGroup returns true
        if (name.compare("")!=0 && lights.compare("")!=0) {
            //create a group for the user
            group->makeGroup("newdeveloper", address,port,name,lights);
            //prompt the user
            Wt::WMessageBox::show("Success!",
                                  "<p>You have successfully added a group.</p>", Wt::StandardButton::Ok);
        }
    }
    delete addDialog_;
}


void AuthWidget::showGroupModify() {
    modDialog_ = new WDialog("Modify");

    //creating the id text field
    new WText("Group ID:    ",modDialog_->contents());
    id = new Wt::WLineEdit(modDialog_->contents());
    id->setInputMask("09;_");
    new WText("<br />",modDialog_->contents());


    //creating the on text field
    new WText("on(true/false):",modDialog_->contents());
    on = new Wt::WLineEdit(modDialog_->contents());
    on->setInputMask("B");
    new WText("<br />",modDialog_->contents());

    //creating the brightness text field
    new WText("Brightness:  ",modDialog_->contents());
    bri = new Wt::WLineEdit(modDialog_->contents());
    bri->setInputMask("009");
    new WText("<br />",modDialog_->contents());

    //creating the hue text field
    new WText("Hue:         ",modDialog_->contents());
    hue = new Wt::WLineEdit(modDialog_->contents());
    hue->setInputMask("00009");
    new WText("<br />",modDialog_->contents());

    //creating the sat text field
    new WText("Saturation:  ",modDialog_->contents());
    sat = new Wt::WLineEdit(modDialog_->contents());
   sat ->setInputMask("009");
    new WText("<br />",modDialog_->contents());

    //creating the lights text field
    new WText("Light Format: \"1\",\"2\" ",modDialog_->contents());
    new WText("Change lights to:",modDialog_->contents());
    modLights = new Wt::WLineEdit(modDialog_->contents());
    new WText("<br />",modDialog_->contents());

    //creating the ok button
    Wt::WPushButton *ok = new Wt::WPushButton("Ok", modDialog_->contents());
    //sending the wLineEdits to the acceptDialog to process
    id->enterPressed().connect(modDialog_,&Wt::WDialog::accept);
    on->enterPressed().connect(modDialog_,&Wt::WDialog::accept);
    bri->enterPressed().connect(modDialog_,&Wt::WDialog::accept);
    hue->enterPressed().connect(modDialog_,&Wt::WDialog::accept);
    sat->enterPressed().connect(modDialog_,&Wt::WDialog::accept);
    modLights->enterPressed().connect(modDialog_,&Wt::WDialog::accept);
    ok->clicked().connect(modDialog_,&Wt::WDialog::accept);

    //displaying
    modDialog_->finished().connect(this, &AuthWidget::modDialogDone);
    modDialog_->show();
    modDialog_->setMovable(false);


}


void AuthWidget::modDialogDone(Wt::WDialog::DialogCode code){
    string newid,newOn, newBri, newHue,newSat,newLight;
    newid = id->text().toUTF8().c_str();
    //if the on is true
    if ((on->text().toUTF8()).compare("1")==0){
        newOn = "true";
    }
        //else on=false
    else{
        newOn = "false";
    }

    newBri = bri->text().toUTF8().c_str();
    newHue= hue->text().toUTF8().c_str();
    newSat = sat->text().toUTF8().c_str();
    newLight = modLights->text().toUTF8().c_str();
    if(code == Wt::WDialog::Accepted){
        //change the values with what the user entered
        group->changeState(newid,newOn,newBri,newHue,newSat);
        //change the lights in the group
        if(newLight.compare("")!=0){
            group->setGroupLights(newid,newLight);
        }
                Wt::WMessageBox::show("Success!",
                              "<p>You have successfully modified a group.</p>",Wt::StandardButton::Ok);
    }
    delete modDialog_;
}

//GROUP DELETE DIALOG

void AuthWidget::showGroupDelete() {
    //setting the tile of the dialog
    delDialog_ = new WDialog("Delete");

    //creating the group id input and setting a mask of 2 digits
    new WText("Group Id:",delDialog_->contents());
    delGroupID = new Wt::WLineEdit(delDialog_->contents());
    delGroupID->setInputMask("09;_");
    new WText("<br />",delDialog_->contents());

    //creating the ok button and connecting it
    Wt::WPushButton *del = new Wt::WPushButton("Delete", delDialog_->contents());
    delGroupID->enterPressed().connect(delDialog_,&Wt::WDialog::accept);
    del->clicked().connect(delDialog_,&Wt::WDialog::accept);
    //displaying the dialog and setting the onfinish function
    delDialog_->finished().connect(this, &AuthWidget::delDialogDone);
    delDialog_->show();
    delDialog_->setMovable(false);

}


void AuthWidget::delDialogDone(Wt::WDialog::DialogCode code){
    //if ok button was pressed
    if(code == Wt::WDialog::Accepted){
        int i;
        string id = delGroupID->text().toUTF8();
        //delete the group and prompt user
        if (id !=""){
            i =group->deleteGroup(id);
            group->getGroups();
            Wt::WMessageBox::show("Success!",
                                  "<p>You have successfully deleted group " + delGroupID->text().toUTF8()+".</p>",Wt::StandardButton::Ok);
        }
        else {
            Wt::WMessageBox::show("Error!",
                                  "<p>Try a valid group-id.</p>", Wt::StandardButton::Ok);
        }
    }

    delete delDialog_;
}