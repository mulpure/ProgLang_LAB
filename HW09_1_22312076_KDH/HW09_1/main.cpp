/*
솔루션명 : HW09_1_22312076_KDH
소스코드 : main.cpp

프로그램의 목적 및 기능
    - GenericArray 템플릿 클래스 테스트
    - 정수, 실수, 문자열 배열 데이터 입력
    - 정렬 전후 배열 상태 출력

함수 목록
    - main

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/05/14
*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "GenericArray.h"

using namespace std;

/*
* 함수명 : main
* 설  명 : GenericArray 객체 생성 후 데이터 입력 및 정렬 수행
* 매개변수 : 없음
* 반환값 : 프로그램 종료 상태값
*/
int main()
{
    fstream fin_int, fin_dbl, fin_str;//파일 스트림 객체

    fin_int.open("intArray_data.txt");//정수 데이터 파일 열기

    if (fin_int.fail()) {
        cout << "Error in reading int_array data file (intArray_data.txt) !!!" << endl;
        exit(1);//파일 열기 실패 시 종료
    }

    int per_line = 10, sample_lines = 2;//출력 형식 설정

    GenericArray<int> ga_Int("GA_Int");//정수형 배열 객체 생성

    fin_int >> ga_Int;//정수 배열 입력
    fin_int.close();//파일 닫기

    cout << "ga_int before sorting : " << endl;
    ga_Int.printArray(per_line, sample_lines);//정렬 전 출력
    cout << endl;

    ga_Int.insertSort();//삽입 정렬 수행

    cout << "ga_int after sorting : " << endl;
    ga_Int.printArray(per_line, sample_lines);//정렬 후 출력
    cout << endl;


    fin_dbl.open("dblArray_data.txt");//실수 데이터 파일 열기

    if (fin_dbl.fail()) {
        cout << "Error in reading int_array data file (dblArray_data.txt) !!!" << endl;
        exit(1);//파일 열기 실패 시 종료
    }

    GenericArray<double> ga_Dbl("GA_Double");//실수형 배열 객체 생성

    fin_dbl >> ga_Dbl;//실수 배열 입력

    cout << "ga_Dbl before sorting : " << endl;
    ga_Dbl.printArray(per_line, sample_lines);//정렬 전 출력
    cout << endl;

    ga_Dbl.insertSort();//삽입 정렬 수행

    cout << "ga_Dbl after sorting : " << endl;
    ga_Dbl.printArray(per_line, sample_lines);//정렬 후 출력
    cout << endl;

    fin_str.open("strArray_data.txt");//문자열 데이터 파일 열기

    if (fin_str.fail()) {
        cout << "Error in reading int_array data file (strArray_data.txt) !!!" << endl;
        exit(1);//파일 열기 실패 시 종료
    }

    GenericArray<string> ga_str("GA_String");//문자열 배열 객체 생성

    fin_str >> ga_str;//문자열 배열 입력

    cout << "ga_str before sorting : " << endl;
    ga_str.printArray(per_line, sample_lines);//정렬 전 출력
    cout << endl;

    ga_str.insertSort();//삽입 정렬 수행

    cout << "ga_str after sorting : " << endl;
    ga_str.printArray(per_line, sample_lines);//정렬 후 출력
    cout << endl;

    return 0;
}