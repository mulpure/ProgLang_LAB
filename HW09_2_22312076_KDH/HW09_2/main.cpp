/*
솔루션명 : HW09_2_22312076_KDH
소스코드 : main.cpp

프로그램의 목적 및 기능
    - GenericCirQ 템플릿 클래스 테스트
    - 원형 큐에 데이터 삽입 및 삭제 수행
    - enqueue와 dequeue 동작 결과 출력

함수 목록
    - main

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/05/13
*/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "GenericCirQ.h"

using namespace std;

#define NUM_ELEMENTS_PER_ROUND 7//한 라운드당 처리할 데이터 개수
#define QUEUE_CACITY 10//원형 큐 최대 크기

/*
* 함수명 : main
* 설  명 : GenericCirQ 객체 생성 후 enqueue와 dequeue 테스트 수행
* 매개변수 : 없음
* 반환값 : 프로그램 종료 상태값
*/
int main()
{
    GenericCirQ<int> genericIntCirQ("GenericIntCirQ", QUEUE_CACITY);//정수형 원형 큐 생성
    int data = 0;//삽입할 데이터
    int* pE;//삭제된 데이터 포인터

    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < NUM_ELEMENTS_PER_ROUND; i++)//데이터 enqueue
        {
            cout << "EnCirQ (" << setw(3) << data << ") => ";
            genericIntCirQ.enqueue(data);//데이터 삽입
            cout << genericIntCirQ;//현재 큐 상태 출력
            data++;//다음 데이터 증가
        }

        for (int i = 0; i < NUM_ELEMENTS_PER_ROUND; i++)//데이터 dequeue
        {
            cout << "DeCirQ (";
            pE = genericIntCirQ.dequeue();//데이터 삭제
            cout << setw(3) << *pE << ") => ";
            cout << genericIntCirQ;//현재 큐 상태 출력
        }
    }
}