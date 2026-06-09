/*
솔루션명 : HW12_4_22312076_KDH
헤 더 명 : Entry_KV.h

프로그램의 목적 및 기능
    - 해시 맵/딕셔너리에서 사용할 (Key, Value) 쌍을 표현하는 제네릭 클래스 정의

클래스 목록
    - Entry_KV<K, V>

특이사항
    - 템플릿 파라미터 K=std::string, V=Voca 로 주로 사용
    - 연산자 오버로딩(=)을 통해 자기 대입 방지 처리

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/06/05
*/
#ifndef ENTRY_H
#define ENTRY_H
#include <iostream>
#include <iomanip>
#include <string>
#include "Voca.h"

// 해시 맵 한 칸에 저장되는 (key, value) 쌍 클래스
template <typename K, typename V>
class Entry_KV {
    //  형식맞춰 출력
    friend std::ostream& operator<<(std::ostream& fout, const Entry_KV& ent)
    {
        fout.setf(std::ios::left);
        fout << " Entry_KV [key: " << std::setw(8) << ent.key();
        fout.unsetf(std::ios::left);
        fout << ", value{" << ent.value() << "} ]";
        return fout;
    }
public:
    Entry_KV() {}
    Entry_KV(K key, V value) { setKey(key); setValue(value); }

    const K& key()   const { return _key; }
    const V& value() const { return _value; }
    void setKey(const K k) { this->_key = k; }
    void setValue(const V v) { this->_value = v; }
    K getKey() { return this->_key; }
    V getValue() { return this->_value; }

    // 자기 대입 방지 후 키와 값을 복사
    Entry_KV& operator=(Entry_KV right)
    {
        if (this != &right) {
            this->_key = right._key;
            this->_value = right._value;
        }
        return *this;
    }
private:
    K _key;
    V _value;
};
#endif