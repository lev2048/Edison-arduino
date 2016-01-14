/*����Զ������
*/
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <WiFi.h>
//���������status������������������״̬����ʼΪδ����
int status = WL_IDLE_STATUS;
//������������Ķ���server���˿�8080
WiFiServer server(8080);
void setup()
{

	pinMode(8, OUTPUT);
	Serial.begin(9600);
	//��������wifi����
	while (status != WL_CONNECTED) {
		Serial.println("Attempting to connect to Network...");
	//����begin��������һ����ʳΪssid�ڶ���Ϊ����
		status = WiFi.begin("yuan", "987410987410");
		delay(10000);
	}
	//��ȡ����ʾ���ӳɹ�������ip��ַ
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);
	//�������񣬼���8080�˿�
	server.begin();

}

void loop()
{

    //����8080�˿��Ƿ��пͻ��˷�������
	WiFiClient client = server.available();
	//�������������
	if (client)
	{
		Serial.println("new client");
		//������ַ�������������Щ������Ϣ
		String currentLine = "";
		//���ͻ�������ʱ�����Ͻ���������Ϣ
		while (client.connected())
		{
			if (client.available())
			{
				char c = client.read();
				Serial.write(c);
				if (c == '\n') {
					if (currentLine.length() == 0)
					{
						//��ʾ��ҳ
						server.println("<html>");
						server.println("<head>");
						//��ҳ����
						server.println("<title> lev's Garden </title>");
						server.println("<meta http-equiv=\"refresh\"content=\"10;url=/\">");
						server.println("</head>");
						server.println("<body>");
						//������Ϣ
						server.println("<h1>Environment</h1>");
						server.print("Temperature��");
						float valueTemp = analogRead(A0);
						valueTemp = -(70*(valueTemp-6400)/25600-10);
						server.println(valueTemp);
						server.println("<br />");
						server.print("light Sensor Value�� ");
						valueTemp = analogRead(A1);
						server.println(valueTemp);
						server.println("<br />");
						server.println("<br />");
						server.println("<h2>PlantA</h2>");
						//����ʪ�� ��֧������
						server.print("Moisture Sensor Value�� ");
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
						//�յ�on���ͨ�̵���
					}
					if (currentLine.endsWith("GET/Off")) {
						digitalWrite(8, LOW);
						//�յ�off�رռ̵���
					}
				}
			}
		}
		//�Ͽ��ͻ��˺ͷ�����������
		client.stop();
		Serial.println("client disconnected.");
	}
}
