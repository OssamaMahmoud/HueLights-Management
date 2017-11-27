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

    Group();

    //bunch of functions that call http calls in response to user activity
    //so just do the http calls for now and do the interface after
    //works


    //gets all the groups in a comma separated string, keep checking till the groupIdList is not empty
    //like while (empty(this->groupIdList))... wait and then call the get states on each one individually
    void  getGroups();

    //given the group id it can change the state of each of the following, on is a string with value "true" or "false"
    int changeState(string groupId, string on, string bri, string hue, string sat);

    //this amazing function when given a groupID and a string of lights like " "1", "2", ... "n" " will set the lights to the indiccated in string
    int setGroupLights(string groupId, string newLights);

    int deleteGroup(string groupId);

    //this beautiful function when given a groupID returns the state of the group, like the name, the light list, as a string "name, lightID1, lightID2"
    string getState(string groupId);

    string getGroupState(){
        return groupState;
    }
    string getGroupIdList() {
        return groupIdList;
    }


    //makes a god damn group, duhhhhh!
    void makeGroup(string username, string address, string port, string name, string lights);

    const string &getPort() const;

    void setPort(const string &port);

    const string &getId() const;

    void setId(const string &id);

    const string &getName() const;

    void setName(const string &name);

    const string &getLightList() const;

    void setLightList(const string &lightList);

    const string &getUsername() const;

    void setUsername(const string &username);



//    //add a light by getting the light list and appending a light to it, then using the setGroupAttr to change it
//    //works
//    int addLight(string lightID);
//
//    //works
//    int removeLight(string lightID);
//
//    //change states not include value if u don't want to change it




        private:
    std::string address, port, id, name;
    std::string lightList, groupState, groupIdList, username = "newdeveloper";


    string vectorToString(vector<string> vec);


    void handleGetGroups(boost::system::error_code err, const Http::Message &response);


    void handleMakeGroup(boost::system::error_code err, const Http::Message &response);



    void handleChangeState(boost::system::error_code err, const Http::Message &response);



    void handleGetState(boost::system::error_code err, const Http::Message &response);



    void handleSetLights(boost::system::error_code err, const Http::Message &response);
};

#endif //TEAM18_GROUPWIDGET_H


