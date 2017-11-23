//
// Created by ubuntu on 11/7/17.
//

#include "Bridge.h"

using namespace Wt;
using namespace std;
Bridge::Bridge(WContainerWidget *parent):WContainerWidget(parent){

}

Bridge::~Bridge(){

}

/************************************* BRIDGE http request ****************************************/

void Bridge::defaultConnect(string the_address, string the_port, string the_reference) {
    set_address(the_address);
    set_port(the_port);
    set_userName("newdeveloper");
    set_reference(the_reference);


    string url = "http://" + the_address + ":" + the_port + "/api/newdeveloper/";
    set_URL(url);
    //url = "http://localhost:80/api/newdeveloper/";

    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleDefaultResponse,this,_1,_2));

    //responseBody = "response body";
    set_defaultResponse(responseBody);

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

void Bridge::newUserConnect(string the_address, string the_port, string devicetype, string the_username, string the_reference) {
    set_reference(the_reference);
    set_userName(the_username);
    set_address(the_address);
    set_port(the_port);

    string url = "http://" + the_address + ":" + the_port + "/api";
    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleNewUserResponse,this,_1,_2));

    string JSONformat_info = R"({"devicetype": ")" +devicetype + R"(", "name": ")" + the_username + R"("})";
    Http::Message info = Http::Message();
    info.addBodyText(JSONformat_info);

    if(client->post(url, info)){
        set_success("correct url");
        WApplication::instance()->deferRendering();
    }
    else{
        set_success("wrong format");
        //print an error message to the screen
        cerr << "can't open url" << endl;
    }
}

bool Bridge::modifyBridge(string address, string port, string username, string reference) {
    testBridge(address, port, username);
    string result = get_modifyResponse();
    if(get_success() != "correct url"){
        return false;
    }else{
        set_userName(username);
        set_address(address);
        set_port(port);
        set_reference(reference);
        set_URL("http://"+address+":"+port+"/api/newdeveloper/");
        return true;
    }

}

void Bridge::testBridge(string the_address, string the_port, string the_username) {

    string url = "http://" + the_address + ":" + the_port + "/api/" + the_username + "/";
    Http::Client *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleModifyResponse,this,_1,_2));

    if(client->get(url)){
        set_success("correct url");
        WApplication::instance()->deferRendering();
    }
    else{
        set_success("wrong format");
        //print an error message to the screen
        cerr << "can't open url" << endl;
    }
}


/************************************* LIGHT http request ****************************************/

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
        responseBody = response.body();

    }
    else {
        set_success("wrong url");
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }

}

void Bridge::handleNewUserResponse(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();

    if (!err && response.status() == 200){
        set_newUserResponse(response.body());
    }
    else {
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }

}

void Bridge::handleModifyResponse(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();

    if (!err && response.status() == 200){
        set_modifyResponse(response.body());
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
    }
    else{
        set_lightSuccess("wrong url");
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }
}

void Bridge::handleChangeLightNameResponse(boost::system::error_code err, const Http::Message &response){
    if(!err && response.status() == 200){
        string content = "result of changing light name: " + response.body();
        set_newLightNameResponse(content);

    }
    else{
        set_lightSuccess("wrong url");
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }
}

void Bridge::handleChangeLightTurnResponse(boost::system::error_code err, const Http::Message &response) {
    if(!err && response.status() == 200){
        string content = "result of turning the light: " + response.body();
        set_newLightTurnResponse(content);

    }
    else{
        set_lightSuccess("wrong url");
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }
}

void Bridge::handleChangeLightColourResponse(boost::system::error_code err, const Http::Message &response) {
    if(!err && response.status() == 200){
        string content = "result of changing the light colour: " + response.body();
        set_newLightColourResponse(content);

    }
    else{
        set_lightSuccess("wrong url");
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }
}

void Bridge::handleChangeLightBrightnessResponse(boost::system::error_code err, const Http::Message &response) {
    if(!err && response.status() == 200){
        string content = "result of changing the light brightness: " + response.body();
        set_newLightBrightnessResponse(content);

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
        result = result + "," + *it;
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



