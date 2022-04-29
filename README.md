# Diego's Arduino Project!

해당 프로젝트는 마이키 프리미엄을 DIY로 설치하고 링크를 달고싶은데..

단말기도 비싸고.. 매달 돈도 내야하고해서

마이키 프리미엄이 조금 멀리서 스마트키로 잠금 두번 누르면 원격시동되니..

차 트렁크에 0원 유심+LTE라우터 넣고 MQTT로 접점으로 스마트키 버튼 눌러주면

그기능이나 그기능이나 동일할꺼라 생각해서 시작함.

한달째 사용중에 아주 만족하며

차량 메인배터리 소모량도 적어 굿임.

라우터는 중국산 벤통 M100 알리에서 구매해서 혹시나 내장배터리 폭발할까봐 분리하고

12 to 5v 모듈로 wemos와 lte라우터에 직결해서 사용중.




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
