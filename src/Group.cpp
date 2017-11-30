/*!
 * \brief Class that makes POST and GET calls to the Emulator
 * @author Ossama
 */

#include "Group.h"
#include "boost/algorithm/string.hpp"

/*!
 * \brief default constructor
 */
Group::Group(){};


/*!
 *  \brief Creates a group and posts its to the url given via a post request
 * @param username
 * @param address
 * @param port
 * @param name
 * @param lights
 * @author Ossama
 */
void Group::makeGroup(std::string username, std::string address, std::string port, std::string name, std::string lights) {

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups";
    std::cout<< "the URL is:   " << url << std::endl;
    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleMakeGroup,this,_1,_2));
    //create json to send
    string JSON_string = R"({"lights": [)" +
                        (lights) + R"(], "name": ")" + name +
                         R"(", "type": ")" +
                         "LightGroup" + R"("})";

    Http::Message body = Http::Message();
    body.addBodyText(JSON_string);


    if (client->post(url, body)){
        WApplication::instance()->deferRendering();

        this->id = "";

    }

}
/*!
 * \brief handles the request received back from the emulator
 * @param err
 * @param response
 * @author Ossama
 */
void Group::handleMakeGroup(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();

    Wt::Json::Array obj;
    Wt::Json::parse(response.body(),obj);
    cout<< "the response is" << response.body() <<endl;


    if (!err && response.status() == 200) {
        //get the json from the response and then extract the id from it
        this->id = "";
        //cout << "\n\n\nFROM THE HANDLERE WE GOT THIS TING   " << string(obj.get("id").toString());
        Wt::Json::Object x = obj[0];
        Wt::Json::Object y = x.get("success");
        string responseID = string(y.get("id").toString()).substr(8);
        cout << "THE RESPOSNSEID:      " << responseID <<endl;
        this->id.append(responseID);
    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
    }
}



/*!
 * \brief takes a string for group id and sets that respective group's lightstring to the light string given through a put request
 * via a post request
 * @param groupId
 * @param newLights
 * @return
 * @author Ossama
 */
int Group::setGroupLights(string groupId, string newLights) {

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + groupId;
    std::cout<< "the URL is:   " << url << std::endl;

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleSetLights,this,_1,_2));

    string JSON_string = R"({"lights": [)" + (newLights) + R"(])" + R"(})";

    Http::Message body = Http::Message();
    body.addBodyText(JSON_string);

    if (client->put(url, body)){
      WApplication::instance()->deferRendering();


    }

    return 0;
}


/*!
 * \brief handles the setLights response from the emulator
 * @param err
 * @param response
 * @author Ossama
 */
void Group::handleSetLights(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();
    Wt::Json::Array obj;
    Wt::Json::parse(response.body(),obj);


    if (!err && response.status() == 200) {
        //nothing rlly to update
    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
    }
}

/*!
 * \brief given the group id it can change the state of each of the following, on is a string with value "true" or "false"
 *  via a put request
 * @param groupId
 * @param on
 * @param bri
 * @param hue
 * @param sat
 * @return
 * @author Ossama
 */

int Group::changeState(string groupId, string on, string bri, string hue, string sat) {
    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + groupId + "/action";

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleChangeState, this,_1,_2));

    string JSON_string = R"({"on": ")" + std::string(on) +
            "\", \"bri\": " + bri +
            ", \"hue\": " + hue  +
            ", \"sat\": " + (sat) +
            R"(})";

    Http::Message body = Http::Message();
    body.addBodyText(JSON_string);

    if (client->put(url, body)){
        WApplication::instance()->deferRendering();

        //update the state by getting it again,
    }

    return 0;
}

/*!
 * \brief handle the request sent back from the server
 * @param err
 * @param response
 * @author Ossama
 */
void Group::handleChangeState(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();
    Wt::Json::Array obj;
    Wt::Json::parse(response.body(),obj);


    if (!err && response.status() == 200) {
        //nothing
    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
    }
}

/*!
 * \brief delete a group via a delete request with the id
 * @param groupId
 * @return
 */
int Group::deleteGroup(string groupId){

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + groupId ;

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleChangeState, this,_1,_2));

    Http::Message body = Http::Message();

    if (client->deleteRequest(url, body)){
        WApplication::instance()->deferRendering();
        //nothing
        return 0;
    }

    return 1;
}

/*!
 * \brief get all groups from the bridge via a get request
 * gets all the groups in a comma seperated string, keep checking till the groupIdList is not empty
 * like while (empty(this->groupIdList))... wait and then call the get states on each one individually
 * @author Ossama
 */
void Group::getGroups(){
    string url = "http://" + address + ":" + port + "/api/" + username + "/groups" ;

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleGetGroups, this,_1,_2));

    if (client->get(url)){

        WApplication::instance()->deferRendering();

        this->groupIdList = "";

    }

}

/*!
 * \brief handle the request sent back from the emulator
 * @param err
 * @param response
 * @author Ossama
 */
void Group::handleGetGroups(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();


    if (!err && response.status() == 200) {
        Wt::Json::Object obj;

        Wt::Json::parse(response.body(),obj);
        //get the json from the response and then extract the id from it

        set<string> names = obj.names();
        for( auto id  : names){
            cout << "please wiork   " << string(id) << endl;
            //this->getState(string(id));

            this->groupIdList.append(string(id) + ",");
        }

        //remove the last comma
        if(groupIdList.size() > 1)
            this->groupIdList = this->groupIdList.substr(0, groupIdList.length() - 1);

        cout << "our final string is for the groups is:  " << this->groupIdList;

    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
    }
}


/*!
 * \brief when given a groupID returns the state of the group, like the name, the light list, as a string "name, lightID1,
 * lightID2" via a get request
 * @param groupId
 * @return
 * @author Ossama
 */
std::string Group::getState(string groupId) {
    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + groupId ;

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleGetState, this,_1,_2));

    //just throwing in null for fun, idk what is supposed to in theres
    if (client->get(url)){
       WApplication::instance()->deferRendering();

        //TODO: something i guess

    }

    return "success";
}

/*!
 * \brief handle the request sent back from the emulator
 * @param err
 * @param response
 */
void Group::handleGetState(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();
    Wt::Json::Object obj;
    Wt::Json::parse(response.body(), obj);


    if (!err && response.status() == 200) {
        //get the json from the response and then extract the id from it;
        this->groupState = "";
        this->groupState.append(string("Name: ") + string(obj.get("name").toString()) + ", Lights: ");
        Wt::Json::Array lightsArray = obj.get("lights");
        //for loop go through the  array and make str of it
        for (int i = 0; i < lightsArray.size(); i++) {
            if (i == 0)
                this->groupState.append("\"" + string(lightsArray[i].toString()) + "\"");
            else
                this->groupState.append(", \"" + string(lightsArray[i].toString()) + "\"");

        }
    }
}

/*!
 * \brief returns all states of the groups
 * @param groupId
 * @return
 */
std::string Group::getAllState(string groupId) {
    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + groupId ;

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleAllGetState, this,_1,_2));

    if (client->get(url)){
       WApplication::instance()->deferRendering();


    }

    return "s";
}

/*!
 * \brief handle the request from the getAllState
 * @param err
 * @param response
 */
void Group::handleAllGetState(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();
    Wt::Json::Object obj;
    Wt::Json::parse(response.body(), obj);

    if (!err && response.status() == 200) {
        //get the json from the response and then extract the id from it
        //this->lightList = "";
        this->groupState.append(string(obj.get("name").toString()) + ", Lights: \n");
        Wt::Json::Array lightsArray = obj.get("lights");
        //for loop go through the  array and make str of it
        for (int i = 0; i < lightsArray.size(); i++) {
            if (i == 0)
                this->groupState.append("\"" + string(lightsArray[i].toString()) + "\"");
            else
                this->groupState.append(", \"" + string(lightsArray[i].toString()) + "\"");

        }
        this->groupState.append("|");
    }
}


/*!
 * \brief get the port
 * @return
 */
const string &Group::getPort() const {
    return port;
}
/*!
 * \brief set the port of the groups
 * @param port
 */
void Group::setPort(const string &port) {
    Group::port = port;
}

/*!
 * \brief get Id for the group
 * @return
 */
const string &Group::getId() const {
    return id;
}

/*!
 * \brief set the id for the group
 * @param id
 */
void Group::setId(const string &id) {
    Group::id = id;
}

/*!
 * \brief get the name
 * @return
 */
const string &Group::getName() const {
    return name;
}

/*!
 * \brief set the name
 * @param name
 */
void Group::setName(const string &name) {
    Group::name = name;
}

/*!
 * \brief get the light list of the group
 * @return
 */
const string &Group::getLightList() const {
    return lightList;
}

/*!
 * \brief set the list of lights
 */
void Group::setLightList(const string &lightList) {
    Group::lightList = lightList;
}

/*!
 * \brief get the username
 * @return
 */
const string &Group::getUsername() const {
    return username;
}

/*!
 * \brief set the username
 * @param username
 */
void Group::setUsername(const string &username) {
    Group::username = username;
}
/*!
 * \brief set the address
 * @param address
 */
void Group::setAddress(const string &address){
    this->address = address;
}
/*!
 * \brief get the address
 * @return
 */
const string &Group::getAddress() const {
    return address;
}
/*!
 * \brief get the group states
 * @param groupState
 */
void Group::setGroupState(const string &groupState) {
    Group::groupState = groupState;
}

/*!
 * \brief set the idList
 * @param groupIdList
 */
void Group::setGroupIdList(const string &groupIdList) {
    Group::groupIdList = groupIdList;
}
/*!
 *  \brief get the group state
 * @return
 */
string Group::getGroupState(){
    return groupState;
}
/*!
 * \brief get the groupidList
 * @return
 */
string Group::getGroupIdList() {
    return groupIdList;
}