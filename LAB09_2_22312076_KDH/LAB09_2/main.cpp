/*
솔루션명 : LAB09_2_22312076_KDH
소스코드 : main.cpp

프로그램의 목적 및 기능
    - 도시 간 거리 데이터 파일 입력
    - 도시 간 거리 테이블 생성
    - 전체 도시 거리표 출력

함수 목록
    - main

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/05/14
*/


#include "InterCityDist.h"

using namespace std;

/*
* 함수명 : main
* 설  명 : 도시 간 거리 데이터를 입력받아 거리표 출력
* 매개변수 : 없음
* 반환값 : 프로그램 종료 상태값
*/
int main() {
    ifstream fin;//입력 파일 스트림

    map<pair<string, string>, int> interCityDistMap;//도시 간 거리 저장 map
    set<string> cities;//도시 이름 저장 set

    fin.open("KR_InterCityDist_11.txt");//거리 데이터 파일 열기

    //파일 여는데 실패하면 출력
    if (fin.fail()) {
        cout << "Error in opening KR_InterCityDist_11.txt !!\n";
        return -1;
    }

    fgetCityPairDistTbl(fin, cities, interCityDistMap);//거리 데이터 입력

    printDistanceTable(interCityDistMap, cities);//전체 도시 거리표 출력

    return 0;
}