//
// Created by ubuntu on 11/21/17.
//

#ifndef TEAM18_GROUPWIDGET_H
#define TEAM18_GROUPWIDGET_H


#include <Wt/WContainerWidget>
#include <vector>
#include "Session.h"

//TODO change to just group, and implement it properly with id as value
//and get all groups can be put in the view

class Group {

public:

    //bunch of functions that call http calls in response to user activity
    //so just do the http calls for now and do the interface after
    //works
    Group(string username, string address, string port, string name, string lights);

    const string &getPort() const;

    void setPort(const string &port);

    const string &getId() const;

    void setId(const string &id);

    const string &getName() const;

    void setName(const string &name);

    void setState(const string &state);

    const string &getLightList() const;

    void setLightList(const string &lightList);

    const string &getUsername() const;

    void setUsername(const string &username);


    //add a light by getting the light list and appending a light to it, then using the setGroupAttr to change it
    //works
    int addLight(string lightID);

    //works
    int removeLight(string lightID);

    //change states not include value if u don't want to change it
    int changeState(bool on, int bri = -1, int hue = -1, int sat = -1);


    int deleteGroup();

    //gets the json of the info as described in the hue api

    string getAddress();

    string setAddress();

    //does not work
    void  getAllGroups();

    //works
    string getState();
    int changeState(string on, string bri, string hue, string sat);



        private:
    std::string address, port, id, name, on, bri, sat, hue;
    std::string lightList, username = "newdeveloper";


    string vectorToString(vector<string> vec);


    void handleMakeGroup(boost::system::error_code err, const Http::Message &response);


    void handleUpdateLights(boost::system::error_code err, const Http::Message &response);

    void handleChangeState(boost::system::error_code err, const Http::Message &response);

    int setGroupLights(string newLights);


    void handleGetState(boost::system::error_code err, const Http::Message &response);

};

#endif //TEAM18_GROUPWIDGET_H


