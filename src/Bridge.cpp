//
// Created by ubuntu on 11/7/17.
//

#include "Bridge.h"

using namespace Wt;
using namespace std;
Bridge::Bridge() {

	//the page where the emulator is sending out the bridge json
	string url_emulator = "http://localhost:8080/api/newdeveloper";
	Http::Client *client = new Http::Client(this);
	client->setTimeout(2);
	client->setMaximumResponseSize(100*1024);
	client->done().connect(boost::bind(&Bridge::handleHttpResponse,this,_1,_2));
	
	//getting the json from the emulator through a http get request
	if (client->get(url_emulator)) 
		cout<<"getting webpage"<<endl;
	else{
		cout << "can't open url" << endl;
	}
}

Bridge::~Bridge(){
}
void Bridge::handleHttpResponse(boost::system::error_code err, const Http::Message& response)  {
	//if there was no error connecting and we're on the correct page
	if (!err && response.status() == 200){
		cout<< response.body() << endl;

		//parse the string received by the http get method
		Json::parse(response.body(), bridgeData, err);
		/*cout << "after parse "<< err << endl;
		Json::Object lights = bridgeData.get("lights");
		Json::Object num = lights.get("1");
		string name = num.get("name");
		cout << name << endl;*/
	}
	else {
		cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
	}
}