

#ifndef TEAM18_GROUPWIDGET_H
#define TEAM18_GROUPWIDGET_H


#include <Wt/WContainerWidget>
#include <vector>
#include "Session.h"


/*
 * @brief creates, deletes, changesstate and getstate of a group in a bridge
 * @author Ossama
 *
 */
class Group {

public:

    /*
     * @brief constructor for group
     * @params
     * @returns
     */
    Group();

    //bunch of functions that call http calls in response to user activity
    //so just do the http calls for now and do the interface after
    //works


    /*
     * @brief gets all the groups in a comma separated string and sets the groupLightID string
     * @params
     * @returns
     */
    void  getGroups();

    /*
     * @brief change the state of the group,
     * @params string groupid of group, string on to indicate "true/false" for light state, string sat, string hue
     * @returns int
     */
    int changeState(string groupId, string on, string bri, string hue, string sat);

    /*
    * @brief set the lights to the indiccated in string
    * @params groupID and a string of lights like "1", "2", ... "n"
    * @returns int success
    */
    int setGroupLights(string groupId, string newLights);

    /*
     * @brief deletes the group
     * @params groupid of group to be deleted
     * @returns success
     */
    int deleteGroup(string groupId);

    //this beautiful function when given a groupID returns
    /*
     * @brief sets var groupstate to the state of the group, like the name, the light list, as a string "name, lightID1, lightID2"
     * @params group id in question
     * @returns success string
     */
    string getState(string groupId);

    /*
    * @brief makes a group using the http call
    * @params username of user, address of bridge, port of bridge, name of group, list of lights as a string
    * @returns
    */
    void makeGroup(string username, string address, string port, string name, string lights);


    //GETTERS AND SETTERS
    string getGroupState();

    string getGroupIdList();

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

    const string &getAddress() const;


    void setAddress(const string &address);

    void setGroupState(const string &groupState);

    void setGroupIdList(const string &groupIdList);


        private:
    std::string address, port, id, name;
    std::string lightList, groupState, groupIdList, username = "newdeveloper";




    void handleGetGroups(boost::system::error_code err, const Http::Message &response);




    void handleChangeState(boost::system::error_code err, const Http::Message &response);


    void handleMakeGroup(boost::system::error_code err, const Http::Message &response);


    void handleGetState(boost::system::error_code err, const Http::Message &response);




    void handleSetLights(boost::system::error_code err, const Http::Message &response);

    string getAllState(string groupId);


    void handleAllGetState(boost::system::error_code err, const Http::Message &response);
};

#endif //TEAM18_GROUPWIDGET_H


