//
// Created by Sharll on 2022/12/27.
//
#include "iostream"
#include "fstream"
#include "tinyjson.hpp"

using namespace std;
using namespace tiny;

string Os_Name = "windows";
string Os_Sep = "\\";
string OS_Obj_Sep = ";";

string read(string file){
	ifstream infile;
	infile.open(file, ios::in);
	if(!infile.is_open()) return "";
	string buf;
	string ret;
	while(getline(infile, buf)){
		ret += buf + "\n";
	}
	return ret;
}

string ClassToPath(string Class){
	string recent;
	int id = 0;
	string Package;
	string Name;
	string Version;
	for(char i: Class){
		if(i==':'){
			if(id == 0) Package = recent;
			else if(id == 1) Name = recent;
			id++;
			recent = "";
		}else if(i == '.' && id == 0){
			recent += Os_Sep;
		}else{
			recent += i;
		}
	}
	Version = recent;
	return Package + Os_Sep + Name + Os_Sep + Version + Os_Sep + Name + "-" + Version + ".jar";
}

void launch(string McPath, string version) {
	string version_root_path = McPath + Os_Sep + "versions" + Os_Sep + version;
	string library_root_path = McPath + Os_Sep + "libraries";

	string basicString = "java";

	basicString += " -Djava.library.path=" + version_root_path + Os_Sep + "natives-" + Os_Name;

	string JsonString = read(version_root_path + Os_Sep + version + ".json");
	TinyJson json;
	json.ReadJson(JsonString);

	basicString += " -cp ";

	xarray libs = json.Get<xarray>("libraries");
	for(int i=0; i < libs.Count(); i++){
		libs.Enter(i);
		basicString += library_root_path + Os_Sep + ClassToPath(libs.Get<string>("name")) + OS_Obj_Sep;
	}

	basicString += version_root_path + Os_Sep + version +".jar"; // 将Minecraft客户端加入classpath
	basicString += " " + json.Get<string>("mainClass");					 // 将主类加入要执行的命令
	basicString += " " + json.Get<string>("minecraftArguments"); // 将游戏参数添加在指令末尾

	cout << basicString;

	system(basicString.c_str()); // system()启动游戏
}
