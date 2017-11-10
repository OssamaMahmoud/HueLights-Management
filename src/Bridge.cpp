//
// Created by ubuntu on 11/7/17.
//

#include "Bridge.h"

using namespace Wt;
using namespace std;
Bridge::Bridge() {

	string url_emulator = "http://localhost:8080/api/newdeveloper";
	Http::Client *client = new Http::Client(this);
	client->setTimeout(2);
	client->setMaximumResponseSize(100*1024);
	client->done().connect(boost::bind(&Bridge::handleHttpResponse,this,_1,_2));
	if (client->get(url_emulator)) 
		cout<<"getting webpage"<<endl;
	else{
		cout << "can't open url" << endl;
	}
}

Bridge::~Bridge(){
}
void Bridge::handleHttpResponse(boost::system::error_code err, const Http::Message& response)  {
	if (!err && response.status() == 200){
		cout<< response.body() << endl;

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