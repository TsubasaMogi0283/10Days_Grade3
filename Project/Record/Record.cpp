#include "Record.h"

Record* Record::GetInstance(){
	static Record record;

	return &record;
}

void Record::ResetScore(){
	//0にリセット
	totalScore_ = 0;
}

void Record::AddRockScore(){
	//加算
	totalScore_ += ROCK_POINT_;
}

void Record::AddFeScore(){
	//減点
	totalScore_ += FE_POINT_;
}

void Record::DeductionScore(){
	//減点
	totalScore_ += DEDUCATION_POINT_;
}
