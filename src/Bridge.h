/*!
 * \brief Header class for Bridge.cpp
 */
#ifndef AUTH_BRIDGE_H

#define AUTH_BRIDGE_H

#include <Wt/WApplication>
#include <Wt/Http/Response>
#include <Wt/WServer>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/Http/Client>
#include <Wt/Http/Request>
#include <Wt/Json/Object>
#include <Wt/Json/Array>
#include <Wt/Json/Value>
#include <Wt/Json/Parser>
#include <Wt/WResource>

#include <string>

#include "User.h"


/*
 * Bridge class:
 *
 * main function1: connect to hue bridge, test the url is correct or not
 *
 * main function2: view lights, view one specified light, and change light attributes
 */

using namespace Wt;
using namespace std;
namespace dbo = Wt::Dbo;

class User;

class Bridge: public WContainerWidget{
public:
    explicit Bridge(WContainerWidget *parent = 0);
    virtual ~Bridge();
    bool emptyObj;
    Json::Object bridgeData;

    dbo::ptr<User> user;

    template<class Action>
    void persist(Action& a)
    {
        dbo::belongsTo(a, user, "user");
    }


    /* use defualt user name to connect bridge */
    void defaultConnect(string address, string port, string reference);
    /*
     * modify address, port, username, or reference
     * will call testBridge after the change
     */
    void modifyBridge(string address, string port, string username, string reference);
    /*
     * to test after:
     * create new user name
     * or edit port, address, or user name
     */
    void testBridge(string address, string port, string username);

    /*
     * functions for light
     */
    void get_allLights();
    void get_oneLight(string light_id);
    void change_lightName(string light_id, string light_name);
    void change_lightTurn(string light_id, string trueORfalse);
    void change_lightColour(string light_id, string colourCode);//colour from 0 to 65280
    void change_lightBrightness(string light_id, string brightness);//brightness from 0 to 254

    /*
     * temp attributes when modifying the bridge
     * store to temp, check if correct, then store into URL, port, username, and reference
     */
    void set_tempAdd(string add);
    string get_tempAdd();
    void set_tempPort(string port);
    string get_tempPort();
    void set_tempUser(string user);
    string get_tempUser();
    void set_tempRef(string ref);
    string get_tempRef();

    /*
     * getters and setters
     */
    void set_reference(string the_reference);
    string get_reference(); //the reference of a bridge
    void set_userName(string the_name);
    string get_userName(); //the username
    void set_address(string the_address);
    string get_address(); //the ip address
    void set_port(string the_port);
    string get_port(); //the port number
    void set_defaultResponse(string response);
    string get_defaultResponse(); //the response after connecting to the hue bridge (e.g. "http://localhost:80/api/newdeveloper/")
    void set_newUserResponse(string response);
    string get_newUserResponse(); // the response after creating new bridge username other than "newdeveloper"(this is not used...)
    void set_modifyResponse(string response);
    string get_modifyResponse(); // the response after modifying the bridge
    void set_URL(string the_url);
    string get_URL(); //the URL of the bridge
    void set_success(string flag);
    string get_success(); //whether the url is correct to connect to hue bridge is indicated here
    void set_modifySuccess(string flag);
    string get_modifySuccess(); //whether the url is correct to modify the hue bridge is indicated here
    void set_lightSuccess(string flag);
    string get_lightSuccess(); //whether the url is correct to do things with lights is indicated here
    void set_lightSet(set<string> set);
    string get_lightSet(); //all the lights in the bridge
    set<string> get_testSet(); //all the set of strings of light id
    void set_oneLightContent(string content);
    string get_oneLightContent(); //the content of one specified light
    void set_newLightNameResponse(string response);
    string get_newLightNameResponse(); // response after changing the light name
    void set_newLightTurnResponse(string response);
    string get_newLightTurnResponse(); // response after truning on or off the light
    void set_newLightColourResponse(string response);
    string get_newLightColourResponse(); // response after changing the light colour
    void set_newLightBrightnessResponse(string response);
    string get_newLightBrightnessResponse(); // response after changing the light brightness


private:
    /*
     * HTTP handler
     */
    void handleDefaultResponse(boost::system::error_code err, const Http::Message& response);
    void handleModifyResponse(boost::system::error_code err, const Http::Message& response);
    void handleAllLightsResponse(boost::system::error_code err, const Http::Message &response);
    void handleOneLightResponse(boost::system::error_code err, const Http::Message &response);
    void handleChangeLightNameResponse(boost::system::error_code err, const Http::Message &response);
    void handleChangeLightTurnResponse(boost::system::error_code err, const Http::Message &response);
    void handleChangeLightColourResponse(boost::system::error_code err, const Http::Message &response);
    void handleChangeLightBrightnessResponse(boost::system::error_code err, const Http::Message &response);

    /*
     * attributes
     */
    set<string> lightSet;
    string newLightNameResponse;
    string newLightTurnResponse;
    string newLightColourResponse;
    string newLightBrightnessResponse;
    string oneLightContent;
    string URL;
    string reference;
    string userName;
    string address;
    string port;
    string defaultResponse;
    string newUserResponse;
    string modifyResponse;
    string responseBody;
    string success;
    string modifySuccess;
    string lightSuccess;

    /*
     * temporary storage of address, port, username, and reference
     */
    string tempAdd;
    string tempPort;
    string tempUser;
    string tempRef;
};


#endif AUTH_BRIDGE_H