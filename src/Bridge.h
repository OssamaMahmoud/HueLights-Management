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
#include <Wt/Json/Parser>
#include <Wt/WResource>

#include <string>

#include "User.h"

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


    /*
     * user optionally give an user name other than "newdeveloper"
     * not required by the project
     * and DO NOT use it
     *
     */
    void newUserConnect(string address, string port, string devicetype, string username, string reference);

    /* use defualt user name to connect bridge */
    void defaultConnect(string address, string port, string reference);
    /*
     * modify address, port, username, or reference
     * will call testBridge after the change
     * return TRUE for success, FALSE for failure
     */
    bool modifyBridge(string address, string port, string username, string reference);
    /*
     * to test after:
     * create new user name
     * or edit port, address, or user name
     */
    void testBridge(string address, string port, string username);

    void get_allLights();
    void get_oneLight(string light_id);
    void change_lightName(string light_id, string light_name);
    void change_lightTurn(string light_id, string trueORfalse);
    void change_lightColour(string light_id, string colourCode);//colour from 0 to 65280
    void change_lightBrightness(string light_id, string brightness);//brightness from 0 to 254

    void set_reference(string the_reference);
    string get_reference();
    void set_userName(string the_name);
    string get_userName();
    void set_address(string the_address);
    string get_address();
    void set_port(string the_port);
    string get_port();
    void set_defaultResponse(string response);
    string get_defaultResponse();
    void set_newUserResponse(string response);
    string get_newUserResponse();
    void set_modifyResponse(string response);
    string get_modifyResponse();
    void set_URL(string the_url);
    string get_URL();
    void set_success(string flag);
    string get_success();
    void set_modifySuccess(string flag);
    string get_modifySuccess();
    void set_lightSuccess(string flag);
    string get_lightSuccess();
    void set_lightSet(set<string> set);
    string get_lightSet();
    void set_oneLightContent(string content);
    string get_oneLightContent();
    void set_newLightNameResponse(string response);
    string get_newLightNameResponse();
    void set_newLightTurnResponse(string response);
    string get_newLightTurnResponse();
    void set_newLightColourResponse(string response);
    string get_newLightColourResponse();
    void set_newLightBrightnessResponse(string response);
    string get_newLightBrightnessResponse();


private:
    void handleDefaultResponse(boost::system::error_code err, const Http::Message& response);
    void handleNewUserResponse(boost::system::error_code err, const Http::Message& response);
    void handleModifyResponse(boost::system::error_code err, const Http::Message& response);
    void handleAllLightsResponse(boost::system::error_code err, const Http::Message &response);
    void handleOneLightResponse(boost::system::error_code err, const Http::Message &response);
    void handleChangeLightNameResponse(boost::system::error_code err, const Http::Message &response);
    void handleChangeLightTurnResponse(boost::system::error_code err, const Http::Message &response);
    void handleChangeLightColourResponse(boost::system::error_code err, const Http::Message &response);
    void handleChangeLightBrightnessResponse(boost::system::error_code err, const Http::Message &response);

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


};


#endif AUTH_BRIDGE_H