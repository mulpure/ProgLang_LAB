/*
솔루션명 : HW09_3_22312076_KDH
소스코드 : main.cpp

프로그램의 목적 및 기능
    - STL priority_queue 테스트
    - Student 객체를 우선순위 기준으로 정렬
    - 정렬 기준과 정렬 방식(ASC, DESC) 확인

함수 목록
    - main
    - printTitle
    - testPriorityQueue

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/05/14
*/


#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <iomanip>
#include "Student.h"

using namespace std;

void printTitle(const string& title);
void testPriorityQueue(const vector<Student>& students, KeyAttr keyAttr,
    SortOrder order, const string& title);

/*
* 함수명 : main
* 설  명 : Student 객체를 다양한 우선순위 기준으로 priority_queue 테스트 수행
* 매개변수 : 없음
* 반환값 : 프로그램 종료 상태값
*/
int main() {
    vector<Student> students = {
        Student("Kim",20230031,3.85),
        Student("Lee",20210015,4.10),
        Student("Park",20220027,3.42),
        Student("Choi",20230005,4.35),
        Student("Han",20210003,3.85),
        Student("Jung",20240011,2.95),
        Student("Song",20220009,4.35)
    };
    //----이름 오름차순 
    testPriorityQueue(students, KeyAttr::NAME, SortOrder::ASC,"Priority by NAME ascending");

    //-----이름 내림차순
    testPriorityQueue(students, KeyAttr::NAME, SortOrder::DESC, "Priority by NAME descending");

    //-----학번 오름차순 
    testPriorityQueue(students, KeyAttr::STID, SortOrder::ASC, "Priority by stID ascending");

    //-----학번 내림차순
    testPriorityQueue(students, KeyAttr::STID, SortOrder::DESC,  "Priority by stID descending");

    //----학점 오름차순 테스트
    testPriorityQueue(students, KeyAttr::GPA, SortOrder::ASC, "Priority by GPA ascending");

    //----학점 내림차순 테스트
    testPriorityQueue(students, KeyAttr::GPA, SortOrder::DESC, "Priority by GPA descending");

    return 0;
}

/*
* 함수명 : printTitle
* 설  명 : 출력 제목과 헤더 출력
* 매개변수 :
*   - title : 출력 제목
* 반환값 : 없음
*/
void printTitle(const string& title) {
    cout << "\n========================================\n";
    cout << title << endl;
    cout << "----------------------------------------\n";

    cout << left << setw(12) << "Name"
        << right << setw(10) << "stID"
        << setw(8) << "GPA" << endl;

    cout << "----------------------------------------\n";
}

/*
* 함수명 : testPriorityQueue
* 설  명 : priority_queue에 Student 객체 저장 및 우선순위 출력
* 매개변수 :
*   - students : Student 객체 벡터
*   - keyAttr : 우선순위 기준
*   - order : 정렬 방식
*   - title : 출력 제목
* 반환값 : 없음
*/
void testPriorityQueue(const vector<Student>& students,
    KeyAttr keyAttr, SortOrder order, const string& title)
{
    priority_queue<Student, vector<Student>, StudentCompare>
        pq(StudentCompare(keyAttr, order));//우선순위 큐 생성

    for (const auto& s : students) {
        pq.push(s);//큐에 Student 객체 삽입
    }

    printTitle(title);

    cout << "[top() : highest priority student]\n";

    if (!pq.empty()) {
        pq.top().print();
    }

    cout << "\n[all students popped in priority order]\n";

    while (!pq.empty()) {
        pq.top().print();//현재 top 출력
        pq.pop();//top 제거
    }
}