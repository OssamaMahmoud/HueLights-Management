/*!
 * \brief The AuthWidget class is the authentication, for when a user signs in
 * This class keeps the user session alive, and allows the suer to navigate through the application
 *
 *
 * @authors Jimmy, Ossama, Yumeng, Marlin
 */
#include "AuthWidget.h"

/*!
 * \brief Create a new view for the user
 *
 * @param session
 */
AuthWidget::AuthWidget(Session& session): Wt::Auth::AuthWidget(Session::auth(), session.users(),
                                                               session.login()),session_(session){
    group = new Group();
}


/*!
 * \brief create the registration view so the user can sign in
 * @param id
 * @return the model to be used
 */
Wt::WWidget *AuthWidget::createRegistrationView(const Wt::Auth::Identity& id){
    RegistrationView *w = new RegistrationView(session_, this);
    Wt::Auth::RegistrationModel *model = createRegistrationModel();
    if (id.isValid())
        model->registerIdentified(id);
    w->setModel(model);
    return w;
}

/*!
 *  \brief creates the bridge view where the user can enter bridge information
 *  @authors Jimmy, Yumeng
 *
 */
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
    //set a mask for the ip address
    bridgeAddress_->setInputMask("009.009.009.009;_");
    bridgeAddress_->setPlaceholderText("enter address");
    //if the ip stored is not null
    if(user->getBridgeIp().compare("")!=0){
        bridgeAddress_->setText((user->getBridgeIp()));
    }
    choosePage->elementAt(1,0)->addWidget(description2);
    choosePage->elementAt(1,1)->addWidget(bridgeAddress_);

    Wt :: WText *description3 = new Wt :: WText("Port: ");
    bridgePort_ = new Wt:: WLineEdit();
    //set a port mask
    bridgePort_->setInputMask("0099;_");
    bridgePort_->setPlaceholderText("enter port");
    //if the port stored for the user is not null
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
/*!
 *  \brief Connecting to the emulator/bridge, if success redirect them to the main view otherwise display error message
 *  @authors Yumeng, Jimmy
 */
// Check to see if bridge information is correct
void AuthWidget::ConnectToBridge() {
    Wt::Dbo::Transaction t(session_);
    dbo::ptr<User> user = session_.user();
    cout << "ip :  " << user->getBridgeIp() << "\n port:  " << user->getBridgePort() << endl;

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

/*!
 *  /brief The main navigation view
 *  @authors Ossama, Jimmy
 */
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
    WPushButton *help = new Wt :: WPushButton("Help");


    //creating the table to add to the page
    table_ = new Wt :: WTable();
    //table_-> setWidth(Wt :: WLength("100%"));

    table_->elementAt(3,3)->addWidget(lights);
    table_->elementAt(3,4)->addWidget(groups);
    table_->elementAt(3,5)->addWidget(schedules);
    table_->elementAt(4,3)->addWidget(help);
    //connect the buttons to the pages
    lights->clicked().connect(this, &AuthWidget::lightPage);
    groups->clicked().connect(this, &AuthWidget::groupPage);
    schedules->clicked().connect(this, &AuthWidget::schedulePage);
    help->clicked().connect(this, &AuthWidget::helpFunction);
    //adding to the page
    bindWidget("back",back);
    bindWidget("header", prompt);
    bindWidget("table", table_);
}

    void AuthWidget::helpFunction(){
        WText *helpText = new WText();
        helpText->setText("Welcome to the Main Page: <br/> Select one of the buttons above to go to the:"
                                  "<br/>Lights page where you can view your lights and change there state"
                                  "<br/>Groups page where you can view all your groups and whats in them, "
                                  "<br/> create new groups of lights and delete groups"
                                  "<br/>Schedules page where you can create schedules for your "
                                  "<br/> lights or groups and even delete them"
                                  "<br/>Thank you for visiting :) :) :) :) :) :) :) :)"
        );
        table_->elementAt(5,5)->addWidget(helpText);
    }


/*!
 *  \brief Views for the lights
 *  @authors Yumeng, Alex
 */
void AuthWidget:: lightPage(){
    setTemplateText(tr("template.loggedinAfterMain"));
    Wt::Dbo::Transaction t(session_);
    dbo::ptr<User> user = session_.user();
    WPushButton *back = new Wt :: WPushButton("Back");
    Wt :: WText *prompt = new Wt :: WText("                 LIGHTS                   ");
    back->clicked().connect(this,&AuthWidget::MainPage);

    Wt :: Orientation orientation1 = Wt :: Vertical;
    Wt :: Orientation orientation2 = Wt :: Horizontal;

    /* The descriptions */
    Wt :: WText *description1 = new Wt :: WText("I want to get all lights");
    Wt :: WText *description2 = new Wt :: WText("Which light do you want to check?");
    Wt :: WText *description3 = new Wt :: WText("I want to give No.");
    Wt :: WText *description4 = new Wt :: WText(" a new name called  ");
    Wt :: WText *description5 = new Wt :: WText("I want to change the color of No.");
    Wt :: WText *description6 = new Wt :: WText("light as ");
    Wt :: WText *description7 = new Wt :: WText(" I want to turn the light");
    Wt :: WText *description8 = new Wt :: WText("(Enter 'true' for ON, or enter 'false' for OFF)");
    Wt :: WText *description9 = new Wt :: WText("I want to change the brightness of No.");
    Wt :: WText *description10 = new Wt :: WText("light as ");
    Wt :: WText *description11 = new Wt :: WText("(Get all lights first, please)");

    /* Add the description to a table */
    table_ = new Wt :: WTable();


    table_ -> setHeaderCount(11, orientation2);

    table_->elementAt(1, 0)->addWidget(description1);
    table_->elementAt(2, 0)->addWidget(description2);
    table_->elementAt(3, 0)->addWidget(description3);
    table_->elementAt(3, 2)->addWidget(description4);
    table_->elementAt(4, 0)->addWidget(description5);
    table_->elementAt(4, 2)->addWidget(description6);
    table_->elementAt(5, 0)->addWidget(description7);
    table_->elementAt(5, 3)->addWidget(description8);
    table_->elementAt(6, 0)->addWidget(description9);
    table_->elementAt(6, 2)->addWidget(description10);
    table_->elementAt(1, 2)->addWidget(description11);

    /* Create get all light button */
    buttonAllLight = new Wt :: WPushButton("Click here");

    /* Create get one specific light button */
    lightIdEdit_ = new Wt:: WLineEdit();
    lightIdEdit_->setMaxLength(2);
    lightIdEdit_->setPlaceholderText("enter light id");
    buttonOneLight = new Wt :: WPushButton("Check now");

    /* Create rename button */
    lightIdName_ = new Wt:: WLineEdit();
    lightIdName_->setMaxLength(2);
    lightIdName_->setPlaceholderText("enter light id");
    renameEdit_ = new Wt:: WLineEdit();
    renameEdit_->setPlaceholderText("name");
    buttonRename = new Wt :: WPushButton("Rename");

    /* Create change color button */
    lightIdColor_ = new Wt:: WLineEdit();
    lightIdColor_->setMaxLength(2);
    lightIdColor_->setPlaceholderText("enter light id");
    colorEdit_ = new Wt:: WLineEdit();
    colorEdit_->setPlaceholderText("color");
    buttonColor = new Wt :: WPushButton("Change color");

    /* Create turn on/off button */
    switchId_= new Wt:: WLineEdit();
    switchId_->setMaxLength(2);
    switchId_->setPlaceholderText("enter light id");
    onOrOff_ = new Wt:: WLineEdit();
    onOrOff_->setPlaceholderText("true/false");
    buttonTurn = new Wt :: WPushButton("Confirm");

    /* Create change brightness button */
    lightIdBrightness_ = new Wt:: WLineEdit();
    lightIdBrightness_->setMaxLength(2);
    lightIdBrightness_->setPlaceholderText("enter light id");
    brightnessEdit_ = new Wt:: WLineEdit();
    brightnessEdit_->setPlaceholderText("brightness");
    buttonBrightness = new Wt :: WPushButton("Change brightness");

    table_->elementAt(1, 4)->addWidget(buttonAllLight);
    table_->elementAt(2, 1)->addWidget(lightIdEdit_);
    table_->elementAt(2, 4)->addWidget(buttonOneLight);
    table_->elementAt(3, 1)->addWidget(lightIdName_);
    table_->elementAt(3, 3)->addWidget(renameEdit_);
    table_->elementAt(3, 4)->addWidget(buttonRename);
    table_->elementAt(4, 1)->addWidget(lightIdColor_);
    table_->elementAt(4, 3)->addWidget(colorEdit_);
    table_->elementAt(4, 4)->addWidget(buttonColor);
    table_->elementAt(5, 1)->addWidget(switchId_);
    table_->elementAt(5, 2)->addWidget(onOrOff_);
    table_->elementAt(5, 4)->addWidget(buttonTurn);
    table_->elementAt(6, 1)->addWidget(lightIdBrightness_);
    table_->elementAt(6, 3)->addWidget(brightnessEdit_);
    table_->elementAt(6, 4)->addWidget(buttonBrightness);

    //redirect
    buttonAllLight-> clicked().connect(this, &AuthWidget :: toWaitPage1);
    buttonOneLight-> clicked().connect(this, &AuthWidget :: toWaitPage2);
    buttonRename-> clicked().connect(this, &AuthWidget :: toRename);
    buttonColor-> clicked().connect(this, &AuthWidget :: toChangeColor);
    buttonTurn-> clicked().connect(this, &AuthWidget :: toTurn);
    buttonBrightness-> clicked().connect(this, &AuthWidget :: toChangeBrightness);

    //make sure get all the lights firstly
    if(getAll == false){
        buttonOneLight->disable();
        buttonRename->disable();
        buttonColor->disable();
        buttonTurn->disable();
        buttonBrightness->disable();
    }

    //some flags to make sure infomation updated
    if(secondaryPage1 == true){
        secondaryPage1 = false;
    }

    if(secondaryPage2 == true){
        lightIdEdit_->emptyText();
        secondaryPage2 = false;
    }
    if(secondaryPage3 == true){
        lightIdName_->emptyText();
        renameEdit_->emptyText();
        secondaryPage3 = false;
    }
    if(secondaryPage4 == true){
        lightIdColor_->emptyText();
        colorEdit_->emptyText();
        secondaryPage4 = false;
    }
    if(secondaryPage5 == true){
        switchId_->emptyText();
        onOrOff_->emptyText();
        secondaryPage5 = false;
    }
    if(secondaryPage6 == true){
        lightIdBrightness_->emptyText();
        brightnessEdit_->emptyText();
        secondaryPage6 = false;
    }

    bindWidget("back",back);
    bindWidget("header", prompt);
    bindWidget("table", table_);
}

/*! \brief a wait page to avoid no response accepted due to response delay
 * @author Yumeng
 */
void AuthWidget :: toWaitPage1(){
    table_->clear();
    getAll = true;

    bridge_.get_allLights();

    Wt :: WText *info = new Wt :: WText("Your request has been sent.");
    buttonWait1 = new Wt :: WPushButton("Click here to check result");
    table_->elementAt(1,0)->addWidget(info);
    table_->elementAt(2,0)->addWidget(buttonWait1);
    buttonWait1-> clicked().connect(this, &AuthWidget :: toGetAllLight);
}

/*!
 * \brief another wait page to avoid no response accepted due to response delay
 * @author Yumeng
 */
void AuthWidget :: toWaitPage2(){
    table_->clear();
    bridge_.get_oneLight(lightIdEdit_->text().toUTF8().c_str());

    Wt :: WText *info = new Wt :: WText("Your request has been sent.");
    buttonWait2 = new Wt :: WPushButton("Click here to check result");
    table_->elementAt(1,0)->addWidget(info);
    table_->elementAt(2,0)->addWidget(buttonWait2);
    buttonWait2-> clicked().connect(this, &AuthWidget :: toGetSpecificLight);

}

/*!
 * \brief method to get all lights
 * @author Yumeng
 */
void AuthWidget :: toGetAllLight() {
    secondaryPage1 = true;
    getAll = true;

    table_->clear();

    Wt :: WText *allLightDescription = new Wt :: WText(bridge_.get_lightSet());
    Wt :: WText *info = new Wt :: WText("You have lights with id: ");

    table_->elementAt(0,0)->addWidget(info);
    table_->elementAt(1,0)->addWidget(allLightDescription);


    lightButton =  new Wt :: WPushButton("Back to light page");
    table_->elementAt(2,0)->addWidget(lightButton);
    lightButton-> clicked().connect(this, &AuthWidget :: lightPage);
}

/*! \brief method to get one specific light information
 * @author Yumeng
 */
void AuthWidget :: toGetSpecificLight() {
    secondaryPage2 = true;

    table_->clear();


    string lightID = lightIdEdit_->text().toUTF8().c_str();
    set<string> lightSet = bridge_.get_testSet();

    if(lightSet.count(lightID) != 0){
        Wt :: WText *info = new Wt :: WText("The status of this light: ");
        Wt :: WText *oneLightDescription = new Wt :: WText(bridge_.get_oneLightContent());
        table_->elementAt(0,0)->addWidget(info);
        table_->elementAt(1,0)->addWidget(oneLightDescription);
    }else{
        Wt :: WText *infoError = new Wt :: WText("Sorry.\n");
        Wt :: WText *errorDescription = new Wt :: WText("wrong light id");
        table_->elementAt(0,0)->addWidget(infoError);
        table_->elementAt(1,0)->addWidget(errorDescription);
    }

    lightButton =  new Wt :: WPushButton("Back to light page");
    table_->elementAt(2,0)->addWidget(lightButton);
    lightButton-> clicked().connect(this, &AuthWidget :: lightPage);

}

/*! \brief method to rename a light
 * @author Yumeng
 */
void AuthWidget :: toRename() {
    secondaryPage3 = true;
    table_->clear();

    bridge_.change_lightName(lightIdName_->text().toUTF8().c_str(),renameEdit_->text().toUTF8().c_str());


    Wt :: WText *info = new Wt :: WText("Your request has been sent.");

    table_->elementAt(0,0)->addWidget(info);

    buttonResult =  new Wt :: WPushButton("Next");
    table_->elementAt(2,0)->addWidget(buttonResult);
    buttonResult-> clicked().connect(this, &AuthWidget ::  toResult);
}

/*! \brief method to change a light's color
 * @author Yumeng
 */
void AuthWidget :: toChangeColor() {
    secondaryPage4 = true;
    table_->clear();

    bridge_.change_lightColour(lightIdColor_->text().toUTF8().c_str(), colorEdit_->text().toUTF8().c_str());


    Wt :: WText *info = new Wt :: WText("Your request has been sent.");
    table_->elementAt(0,0)->addWidget(info);

    buttonResult =  new Wt :: WPushButton("Next");
    table_->elementAt(2,0)->addWidget(buttonResult);
    buttonResult-> clicked().connect(this, &AuthWidget ::  toResult);
}

/*! \brief method to turn on/off a light
 * @author Yumeng
 */
void AuthWidget :: toTurn() {
    secondaryPage5 = true;
    table_->clear();

    bridge_.change_lightTurn(switchId_->text().toUTF8().c_str(), onOrOff_->text().toUTF8().c_str());


    Wt :: WText *info = new Wt :: WText("Your request has been sent.");
    table_->elementAt(0,0)->addWidget(info);

    buttonResult =  new Wt :: WPushButton("Next");
    table_->elementAt(2,0)->addWidget(buttonResult);
    buttonResult-> clicked().connect(this, &AuthWidget :: toResult);
}

/*! \brief  method to change a light's brightness
 * @author Yumeng
 */
void AuthWidget :: toChangeBrightness() {
    secondaryPage6 = true;
    table_->clear();

    bridge_.change_lightBrightness(lightIdBrightness_->text().toUTF8().c_str(), brightnessEdit_->text().toUTF8().c_str());


    Wt :: WText *info = new Wt :: WText("Your request has been sent.");
    table_->elementAt(0,0)->addWidget(info);

    buttonResult =  new Wt :: WPushButton("Next");
    table_->elementAt(2,0)->addWidget(buttonResult);
    buttonResult-> clicked().connect(this, &AuthWidget :: toResult);
}

/*!
 * /brief method to accept the answer from rename, change color, turn on/off, change brightness methods
 * @author Yumeng
 */
void AuthWidget :: toResult(){
    // if call rename method
    if(secondaryPage3 == true){
        table_->clear();

        string lightID = lightIdName_->text().toUTF8().c_str();
        set<string> lightSet = bridge_.get_testSet();

        if(lightSet.count(lightID) != 0){
            Wt :: WText *info = new Wt :: WText("The answer is: ");
            Wt :: WText *oneLightDescription = new Wt :: WText(bridge_.get_newLightNameResponse());
            table_->elementAt(0,0)->addWidget(info);
            table_->elementAt(1,0)->addWidget(oneLightDescription);
        }else{
            Wt :: WText *infoError = new Wt :: WText("Sorry.\n");
            Wt :: WText *errorDescription = new Wt :: WText("wrong light id");
            table_->elementAt(0,0)->addWidget(infoError);
            table_->elementAt(1,0)->addWidget(errorDescription);
        }
    }
    // if call change-color method
    if(secondaryPage4 == true){
        table_->clear();

        string lightID = lightIdColor_->text().toUTF8().c_str();
        set<string> lightSet = bridge_.get_testSet();

        if(lightSet.count(lightID) != 0){
            Wt :: WText *info = new Wt :: WText("The answer is: ");
            if((stoi(colorEdit_->text().toUTF8()) >= 0) && (stoi(colorEdit_->text().toUTF8()) <= 65535)){
                Wt :: WText *colorDescription = new Wt :: WText(bridge_.get_newLightColourResponse());
                table_->elementAt(0,0)->addWidget(info);
                table_->elementAt(1,0)->addWidget(colorDescription);
            }else{
                Wt :: WText *errorDescription = new Wt :: WText("No such color");
                table_->elementAt(1,0)->addWidget(errorDescription);
            }
        }else{
            Wt :: WText *infoError = new Wt :: WText("Sorry.\n");
            Wt :: WText *errorDescription = new Wt :: WText("wrong light id");
            table_->elementAt(0,0)->addWidget(infoError);
            table_->elementAt(1,0)->addWidget(errorDescription);
        }
    }
    // if call turn-on/off method
    if(secondaryPage5 == true){
        table_->clear();

        string lightID = switchId_->text().toUTF8().c_str();
        set<string> lightSet = bridge_.get_testSet();

        if(lightSet.count(lightID) != 0){
            Wt :: WText *info = new Wt :: WText("The answer is: ");

            if((onOrOff_->text().toUTF8() == "true") || (onOrOff_->text().toUTF8() == "false")){
                Wt :: WText *switchDescription = new Wt :: WText(bridge_.get_newLightTurnResponse());

                table_->elementAt(0,0)->addWidget(info);
                table_->elementAt(1,0)->addWidget(switchDescription);
            }else{
                Wt :: WText *errorDescription = new Wt :: WText("wrong format");
                table_->elementAt(1,0)->addWidget(errorDescription);
            }
        }else{
            Wt :: WText *infoError = new Wt :: WText("Sorry.\n");
            Wt :: WText *errorDescription = new Wt :: WText("wrong light id");
            table_->elementAt(0,0)->addWidget(infoError);
            table_->elementAt(1,0)->addWidget(errorDescription);

        }
    }
    // if call change-brightness method
    if(secondaryPage6 == true){
        table_->clear();

        string lightID = lightIdBrightness_->text().toUTF8().c_str();
        set<string> lightSet = bridge_.get_testSet();

        if(lightSet.count(lightID) != 0){
            Wt :: WText *info = new Wt :: WText("The answer is: ");

            if((stoi(brightnessEdit_->text().toUTF8()) >= 0) && (stoi(brightnessEdit_->text().toUTF8()) <= 254)){
                Wt :: WText *brightnessDescription = new Wt :: WText(bridge_.get_newLightBrightnessResponse());

                table_->elementAt(0,0)->addWidget(info);
                table_->elementAt(1,0)->addWidget(brightnessDescription);
            }else{
                Wt :: WText *errorDescription = new Wt :: WText("no such brightness");
                table_->elementAt(1,0)->addWidget(errorDescription);
            }
        }else{
            Wt :: WText *infoError = new Wt :: WText("Sorry.\n");
            Wt :: WText *errorDescription = new Wt :: WText("wrong light id");
            table_->elementAt(0,0)->addWidget(infoError);
            table_->elementAt(1,0)->addWidget(errorDescription);
        }

    }

    // button to go back light page
    lightButton =  new Wt :: WPushButton("Back to light page");
    table_->elementAt(2,0)->addWidget(lightButton);
    lightButton-> clicked().connect(this, &AuthWidget :: lightPage);
}


/*!
 * \brief View for schedules
 * @author Marlin
 */
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
/*!
 *  \brief View for the groups
 *  @authors Ossama, Jimmy
 */
void AuthWidget::groupPage(){
    setTemplateText(tr("template.loggedinAfterMain"));

    //get the user info from the session
    Wt::Dbo::Transaction t(session_);
    dbo::ptr<User> user = session_.user();

    //create buttons
    WPushButton *back = new Wt :: WPushButton("Back");
    back->clicked().connect(this,&AuthWidget::MainPage);

    //create buttons to add to table
    Wt :: WText *prompt = new Wt :: WText("                 GROUPS                   ");
    WPushButton *view = new Wt :: WPushButton("UpdateView");
    WPushButton *add = new Wt :: WPushButton("Add");
    WPushButton *modify = new Wt :: WPushButton("Modify");
    WPushButton *del = new Wt :: WPushButton("Delete");
    WPushButton *getGroupsId = new Wt :: WPushButton("View Groups");


    //creating the table to add to the page
    table_ = new Wt :: WTable();

    //add the buttons
    table_->elementAt(3,3)->addWidget(view);
    table_->elementAt(3,4)->addWidget(add);
    table_->elementAt(3,5)->addWidget(modify);
    table_->elementAt(3,6)->addWidget(del);
    table_->elementAt(4,3)->addWidget(getGroupsId);

    //create displaytable and add to main table
    //used for the view
    displayTable = new Wt :: WTable();
    table_->elementAt(5,7)->addWidget(displayTable);




    //bind each button to its method
    add->clicked().connect(this, &AuthWidget::showGroupAdd);
    modify->clicked().connect(this, &AuthWidget::showGroupModify);
    del->clicked().connect(this, &AuthWidget::showGroupDelete);
    getGroupsId->clicked().connect(this, &AuthWidget::getGroupsIdHandler);
    view->clicked().connect(this,&AuthWidget::viewNow);

    //bind the views to there respective areas in the template
    bindWidget("back",back);
    bindWidget("header", prompt);
    bindWidget("table", table_);


    //get the port/address from the db
    group->setPort(user->getBridgePort());
    group->setAddress(user->getBridgeIp());

    //get the groupID list
    group->getGroups();

}

/*!
 * \brief displays the information brought from the json into the display table
 * @author Ossama
 */
void AuthWidget::viewNow(){
    //get the state string from the previous call to the emulator
    string groupState = group->getGroupState();
    //display it in the display inner table
    WText *display = new WText();
    display->setText("");
    displayTable->clear();
    displayTable->elementAt(0,0)->addWidget(display);
    cout<<groupState<<endl;
    display->setText(groupState);

}

/*!
 *  \brief obtain the state of the group via id
 * @param id
 * @author Ossama
 */
void AuthWidget::individualGroupButton(string id){
    //get the state of the group specified by the id
    group->getState(id);
}

/*!
 * \brief handle the getGroups call
 * @author Ossama
 */
void AuthWidget::getGroupsIdHandler() {
    string ID = group->getGroupIdList();
    cout << "the id is:       " << ID << endl;
    vector<string> idVector;
    displayTable->clear();
    boost::split(idVector, ID, boost::is_any_of(","));
    for (int i = 0; i < idVector.size() && ID != ""; i++) {
        WPushButton *pushButton = new WPushButton("Get Info group: " + idVector[i]);
        boost::trim(idVector[i]);
        pushButton->clicked().connect( boost::bind(&AuthWidget::individualGroupButton, this, idVector[i] ) );
        //table_->hide();
        this->displayTable->elementAt(0, i)->addWidget(pushButton);

    }
}

/*!
 * \brief show the add Groups dialog to the user
 * @author Jimmy
 */
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

/*!
 * \brief handle the addDialog after user clicks the ok button
 * @param code
 * @author Jimmy
 */
void AuthWidget::addDialogDone(Wt::WDialog::DialogCode code){
    if(code == Wt::WDialog::Accepted){
        string name,lights;
        //getting the text from the WLineEdit
        name = addName_->text().toUTF8();
        lights = addLights->text().toUTF8();

        //if makeGroup returns true
        if (name.compare("")!=0 && lights.compare("")!=0) {
            //create a group for the user
            group->makeGroup("newdeveloper", address,port,name,lights);
            group->getGroups();

            //prompt the user
            Wt::WMessageBox::show("Success!",
                                  "<p>You have successfully added a group.</p>", Wt::StandardButton::Ok);

        }
    }
    delete addDialog_;
}

/*!
 * \brief Display a dialog to the user to modify a group
 * @author Jimmy
 */
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
/*!
 * \brief Handle the information passed through the dialog
 * @param code
 * @author Jimmy
 */
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
        Wt::WMessageBox::show("Success!", "<p>You have successfully modified a group.</p>",Wt::StandardButton::Ok);
    }
    delete modDialog_;
}

/*!
 * \brief Show the delete dialog to the user
 * @author Jimmy
 */
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

/*!
 * \brief handle the information passed through the delete dialog
 * @param code
 * @author Jimmy
 */
void AuthWidget::delDialogDone(Wt::WDialog::DialogCode code){
    //if ok button was pressed
    if(code == Wt::WDialog::Accepted){
        string id = delGroupID->text().toUTF8();
        //delete the group and prompt user
        if (id !=""){
            group->deleteGroup(id);
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
