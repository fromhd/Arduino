# Diego's Arduino Project!

구성 : Wemos D1 mini(ESP8266) + 4채널 릴레이 + 현대차 스마트키 + 마이키 프리미엄 + LTE라우터 + MQTT(Mosquitto)

우분투 서버에 MQTT를 설치하고

<mosquitto.conf는 다음과 같이 수정해줌>

//

persistence false

allow_anonymous false

connection_messages true

log_type all

listener 1883

password_file /mosquitto/data/password.txt

//

<password.txt는 다음과 같이 수정해줌> \
아이디:암호

mosquitto_passwd -b password.txt 아이디 암호
명령어로 암호화 하기

wemos의 디지털핀과 릴레이를 연결하고

릴레이 com과 no를 각 스마트키 버튼에 연결한다.

1. 배터리 +
2. 도어락 버튼
3. 도어언락 버튼
4. 트렁크 버튼

..
