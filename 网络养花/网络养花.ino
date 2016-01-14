/*网络远程养花
*/
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <WiFi.h>
//定义个变量status，用来保存网络连接状态，初始为未连接
int status = WL_IDLE_STATUS;
//定义个服务器的对象server，端口8080
WiFiServer server(8080);
void setup()
{

	pinMode(8, OUTPUT);
	Serial.begin(9600);
	//尝试连接wifi网络
	while (status != WL_CONNECTED) {
		Serial.println("Attempting to connect to Network...");
	//调用begin函数，第一个蚕食为ssid第二个为密码
		status = WiFi.begin("yuan", "987410987410");
		delay(10000);
	}
	//获取并显示连接成功后分配的ip地址
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);
	//启动服务，监听8080端口
	server.begin();

}

void loop()
{

    //监听8080端口是否有客户端发送请求
	WiFiClient client = server.available();
	//如果监听到请求
	if (client)
	{
		Serial.println("new client");
		//定义个字符串用来保存这些请求信息
		String currentLine = "";
		//当客户端连接时，不断接收请求信息
		while (client.connected())
		{
			if (client.available())
			{
				char c = client.read();
				Serial.write(c);
				if (c == '\n') {
					if (currentLine.length() == 0)
					{
						//显示网页
						server.println("<html>");
						server.println("<head>");
						//网页标题
						server.println("<title> lev's Garden </title>");
						server.println("<meta http-equiv=\"refresh\"content=\"10;url=/\">");
						server.println("</head>");
						server.println("<body>");
						//环境信息
						server.println("<h1>Environment</h1>");
						server.print("Temperature：");
						float valueTemp = analogRead(A0);
						valueTemp = -(70*(valueTemp-6400)/25600-10);
						server.println(valueTemp);
						server.println("<br />");
						server.print("light Sensor Value： ");
						valueTemp = analogRead(A1);
						server.println(valueTemp);
						server.println("<br />");
						server.println("<br />");
						server.println("<h2>PlantA</h2>");
						//土壤湿度 不支持中文
						server.print("Moisture Sensor Value： ");
						valueTemp = analogRead(A2);
						server.println(valueTemp);
						server.println("<br />");
						server.println("<a href=\"/On\">turn on</a> the pump<br>");
						server.println("<a href=\"/Off\">turn off</a> the pump<br>");
						server.println("<br />");
						server.println("</body>");
						server.println("</html>");
						server.println();
						break;
					}
					else {
						currentLine = "";
					}
				}
				else if (c != '\r') {
					currentLine += c;
					if (currentLine.endsWith("GET/On")) {
						digitalWrite(8, HIGH);
						//收到on则接通继电器
					}
					if (currentLine.endsWith("GET/Off")) {
						digitalWrite(8, LOW);
						//收到off关闭继电器
					}
				}
			}
		}
		//断开客户端和服务器的连接
		client.stop();
		Serial.println("client disconnected.");
	}
}
