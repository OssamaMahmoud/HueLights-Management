/*!
 * \brief Handle the Bridge and Lights JSON response calls from the emulator
 * @Authors Alex
 */

#include "Bridge.h"

using namespace Wt;
using namespace std;
/*!
 * \brief constructor for the bridge
 * @param parent
 *
 */
Bridge::Bridge(WContainerWidget *parent):WContainerWidget(parent){

}

/*!
 * \brief destructor for the bridge
 *
 */
Bridge::~Bridge(){

}

/************************************* BRIDGE http request ****************************************/
/*!
 * \brief handle the url being used to post and get json
 * @param the_address
 * @param the_port
 * @param the_reference
 * @authors Jimmy, Alex
 */

void Bridge::defaultConnect(string the_address, string the_port, string the_reference) {
    set_tempAdd(the_address);
    set_tempPort(the_port);
    set_tempUser("newdeveloper");
    set_tempRef(the_reference);


    string url = "http://" + the_address + ":" + the_port + "/api/newdeveloper/";
    set_URL(url);
    //url = "http://localhost:80/api/newdeveloper/";

    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleDefaultResponse,this,_1,_2));

    if(client->get(get_URL())){
        set_success("correct url");
        WApplication::instance()->deferRendering();
    }
    else{
        set_success("wrong url");
        //print an error message to the screen
        cerr << "can't open url\n\n\n\n\n\n\n\n\n" << endl;
    }
}
/*!
 * \brief modify a bridge with the three strings being passed in
 * @param address
 * @param port
 * @param username
 * @param reference
 * @author Alex
 */
void Bridge::modifyBridge(string address, string port, string username, string reference) {
    set_tempAdd(address);
    set_tempPort(port);
    set_tempUser(username);
    set_tempRef(reference);
    testBridge(address, port, username);
}

/*!
 * \brief Test the bridge for the correct url
 * @param the_address
 * @param the_port
 * @param the_username
 * @author Alex
 */
void Bridge::testBridge(string the_address, string the_port, string the_username) {

    string url = "http://" + the_address + ":" + the_port + "/api/" + the_username + "/";
    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleModifyResponse,this,_1,_2));

    if(client->get(url)){
        set_modifySuccess("correct url");
        WApplication::instance()->deferRendering();
    }
    else{
        set_modifySuccess("wrong format");
        //print an error message to the screen
        cerr << "can't open url" << endl;
    }
}


/************************************* LIGHT http request ****************************************/
/*!
 * \brief get the lights via a getcall
 * @author Alex
 */
void Bridge::get_allLights() {

    string url = get_URL() + "lights";
    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleAllLightsResponse, this,_1,_2));

    if(client->get(url)){
        set_lightSuccess("correct url");
        WApplication::instance()->deferRendering();
    }else{
        set_lightSuccess("wrong format");
        //print an error message to the screen
        cerr << "can't open url" << endl;
    }
}
/*!
 * \brief get a single light
 * @param light_id
 * @author Alex
 */
void Bridge::get_oneLight(string light_id) {

    string url = get_URL() + "lights/" + light_id;
    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleOneLightResponse, this,_1,_2));

    if(client->get(url)){
        set_lightSuccess("correct url");
        WApplication::instance()->deferRendering();
    }else{
        set_lightSuccess("wrong format");
        //print an error message to the screen
        cerr << "can't open url" << endl;
    }
}

void Bridge::change_lightName(string light_id, string light_name) {

    Http::Message name = Wt::Http::Message();
    string a = "{\"name\":";
    string b = "\"";
    string c = "\"}";
    string json = a + b + light_name + c;
    name.addBodyText(json);
    string url = get_URL() + "lights/" + light_id;
    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleChangeLightNameResponse, this,_1,_2));

    if(client->put(url, name)){
        set_lightSuccess("correct url");
        WApplication::instance()->deferRendering();
    }else{
        set_lightSuccess("wrong format");
        //print an error message to the screen
        cerr << "can't open url" << endl;
    }
}

void Bridge::change_lightTurn(string light_id, string trueORfalse) {

    Http::Message turn = Wt::Http::Message();
    string a = "{\"on\":";
    string b = "\"";
    string c = "\"}";
    string json = a + b + trueORfalse + c;
    turn.addBodyText(json);
    string url = get_URL() + "lights/" + light_id + "/state";
    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleChangeLightTurnResponse, this,_1,_2));

    if(client->put(url, turn)){
        set_lightSuccess("correct url");
        WApplication::instance()->deferRendering();
    }else{
        set_lightSuccess("wrong format");
        //print an error message to the screen
        cerr << "can't open url" << endl;
    }
}

void Bridge::change_lightColour(string light_id, string colourCode) {

    Http::Message colour = Wt::Http::Message();
    string a = "{\"hue\":";
    string b = "\"";
    string c = "\"}";
    string json = a + b + colourCode + c;
    colour.addBodyText(json);
    string url = get_URL() + "lights/" + light_id + "/state";
    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleChangeLightColourResponse, this,_1,_2));

    if(client->put(url, colour)){
        set_lightSuccess("correct url");
        WApplication::instance()->deferRendering();
    }else{
        set_lightSuccess("wrong format");
        //print an error message to the screen
        cerr << "can't open url" << endl;
    }
}

void Bridge::change_lightBrightness(string light_id, string brightness) {

    Http::Message bri = Wt::Http::Message();
    string a = "{\"bri\":";
    string b = "\"";
    string c = "\"}";
    string json = a + b + brightness + c;
    bri.addBodyText(json);
    string url = get_URL() + "lights/" + light_id + "/state";
    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleChangeLightBrightnessResponse, this,_1,_2));

    if(client->put(url, bri)){
        set_lightSuccess("correct url");
        WApplication::instance()->deferRendering();
    }else{
        set_lightSuccess("wrong format");
        //print an error message to the screen
        cerr << "can't open url" << endl;
    }
}

/************************************* BRIDGE http handle ******************************************/

void Bridge::handleDefaultResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();

    if (!err && response.status() == 200){
        Json::Object correctJson;
        Json::parse(response.body(), correctJson);


        if(correctJson.contains("lights")){
            set_success("correct url");
            set_address(get_tempAdd());
            set_port(get_tempPort());
            set_userName(get_tempUser());
            set_reference(get_tempRef());
        }else{
            set_success("wrong url");
        }
    }
    else {
        set_success("wrong url");
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }

}

void Bridge::handleModifyResponse(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();

    if (!err && response.status() == 200){
        Json::Object correctJson;
        Json::parse(response.body(), correctJson);


        if(correctJson.contains("lights")){
            set_modifySuccess("correct url");
            set_userName(get_tempUser());
            set_address(get_tempAdd());
            set_port(get_tempPort());
            set_reference(get_tempRef());
            set_URL("http://"+get_address()+":"+get_port()+"/api/newdeveloper/");
        }else {
            set_success("wrong url");
        }
    }
    else {
        set_modifySuccess("wrong url");
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }

}

/************************************* LIGHT http handle ******************************************/

void Bridge::handleAllLightsResponse(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();

    if(!err && response.status() == 200){

        Json::Object content;
        string result = response.body();
        Json::parse(result, content);
        set_lightSet(content.names());
        set_lightSuccess("correct url");

    }
    else{
        set_lightSuccess("wrong url");
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }
}

void Bridge::handleOneLightResponse(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();

    if(!err && response.status() == 200){

        string name = "the light name: ";
        string brightness = "the brightness: ";
        string colour = "the colour: ";
        string on_off ="the light is: ";


        Json::Object content;
        string result = response.body();
        Json::parse(result, content);
        string light_name = content.get("name");//light name
        Json::Object state = content.get("state");
        int bri = state.get("bri");//light brightness
        int hue = state.get("hue");//light colour
        bool on = state.get("on");// on/off

        //update strings
        on_off = on_off + (on ? "on\n" : "off\n");
        name = name + light_name + "\n";
        brightness = brightness + to_string(bri) + "\n";
        colour = colour + to_string(hue) + "\n";

        set_oneLightContent(name + brightness + on_off + colour);
        set_lightSuccess("correct url");

    }
    else{
        set_lightSuccess("wrong url");
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }
}

void Bridge::handleChangeLightNameResponse(boost::system::error_code err, const Http::Message &response){
    WApplication::instance()->resumeRendering();

    if(!err && response.status() == 200){

        set_newLightNameResponse(response.body());
        set_lightSuccess("correct url");

    }
    else{
        set_lightSuccess("wrong url");
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }
}

void Bridge::handleChangeLightTurnResponse(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();

    if(!err && response.status() == 200){

        set_newLightTurnResponse(response.body());
        set_lightSuccess("correct url");

    }
    else{
        set_lightSuccess("wrong url");
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }
}

void Bridge::handleChangeLightColourResponse(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();

    if(!err && response.status() == 200){

        set_newLightColourResponse(response.body());
        set_lightSuccess("correct url");

    }
    else{
        set_lightSuccess("wrong url");
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }
}

void Bridge::handleChangeLightBrightnessResponse(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();

    if(!err && response.status() == 200){

        set_newLightBrightnessResponse(response.body());
        set_lightSuccess("correct url");


    }
    else{
        set_lightSuccess("wrong url");
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }
}


/************************************* getter setter ************************************/
void Bridge::set_userName(string the_name) {
    userName = the_name;
}

string Bridge::get_userName() {
    return userName;
}

void Bridge::set_address(string the_address) {
    address = the_address;
}

string Bridge::get_address() {
    return address;
}

void Bridge::set_port(string the_port) {
    port = the_port;
}

string Bridge::get_port() {
    return port;
}

void Bridge::set_defaultResponse(string response) {
    defaultResponse = response;
}

string Bridge::get_defaultResponse() {
    return defaultResponse;
}

void Bridge::set_newUserResponse(string response) {
    newUserResponse = response;
}

string Bridge::get_newUserResponse() {
    return newUserResponse;
}

void Bridge::set_modifyResponse(string response) {
    modifyResponse = response;
}

string Bridge::get_modifyResponse() {
    return modifyResponse;
}

void Bridge::set_reference(string the_reference) {
    reference = the_reference;
}

string Bridge::get_reference() {
    return reference;
}

void Bridge::set_URL(string the_url) {
    URL = the_url;
}

string Bridge::get_URL() {
    return URL;
}

void Bridge::set_success(string flag) {
    success = flag;
}

string Bridge::get_success() {
    return success;
}

void Bridge::set_modifySuccess(string flag) {
    modifySuccess = flag;
}

string Bridge::get_modifySuccess() {
    return modifySuccess;
}

void Bridge::set_lightSuccess(string flag) {
    lightSuccess = flag;
}

string Bridge::get_lightSuccess() {
    return lightSuccess;
}

void Bridge::set_lightSet(set<string> set) {
    lightSet = set;
}

string Bridge::get_lightSet() {
    string result = "";
    for (set<string>::iterator it=lightSet.begin(); it!=lightSet.end(); ++it) {
        result = result + "\"" + *it + "\"";
        set<string>::iterator dupe = it;
        ++dupe;
        if(dupe!=lightSet.end()){
            result = result + ", ";
        }
    }
    return result;
}

void Bridge::set_oneLightContent(string content) {
    oneLightContent = content;
}

string Bridge::get_oneLightContent() {
    return oneLightContent;
}

void Bridge::set_newLightNameResponse(string response) {
    newLightNameResponse = response;
}

string Bridge::get_newLightNameResponse() {
    return newLightNameResponse;
}

void Bridge::set_newLightTurnResponse(string response) {
    newLightTurnResponse = response;
}

string Bridge::get_newLightTurnResponse() {
    return newLightTurnResponse;
}

void Bridge::set_newLightColourResponse(string response) {
    newLightColourResponse = response;
}

string Bridge::get_newLightColourResponse() {
    return newLightColourResponse;
}

void Bridge::set_newLightBrightnessResponse(string response) {
    newLightBrightnessResponse = response;
}

string Bridge::get_newLightBrightnessResponse() {
    return newLightBrightnessResponse;
}

void Bridge::set_tempAdd(string add) {
    tempAdd = add;
}

string Bridge::get_tempAdd() {
    return tempAdd;
}

void Bridge::set_tempPort(string port) {
    tempPort = port;
}

string Bridge::get_tempPort() {
    return tempPort;
}

void Bridge::set_tempUser(string user) {
    tempUser = user;
}

string Bridge::get_tempUser() {
    return tempUser;
}

void Bridge::set_tempRef(string ref) {
    tempRef = ref;
}

string Bridge::get_tempRef() {
    return tempRef;
}

set<string> Bridge::get_testSet() {
    return lightSet;
}

