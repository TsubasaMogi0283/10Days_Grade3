#include "Record.h"
#include <Input.h>

Record* Record::GetInstance(){
	static Record record;

	return &record;
}

void Record::Update(){
	

#ifdef _DEBUG

	ImGui::Begin("記録(デバッグ用)");
	ImGui::Text("1で1増える");
	ImGui::Text("2で10増える");
	ImGui::Text("3で100増える");
	ImGui::Text("4で1000増える");
	ImGui::Text("5で10000増える");
	if(Input::GetInstance()->IsTriggerKey(DIK_1)==true){
		AddOnePlace();
	}
	else if (Input::GetInstance()->IsTriggerKey(DIK_2) == true) {
		AddTenPlace();
	}
	else if (Input::GetInstance()->IsTriggerKey(DIK_3) == true) {
		AddHundredPlace();
	}
	else if (Input::GetInstance()->IsTriggerKey(DIK_4) == true) {
		AddThousandPlace();
	}
	else if (Input::GetInstance()->IsTriggerKey(DIK_5) == true) {
		AddTenThousandPlace();
	}
	
	ImGui::End();





#endif // _DEBUG



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

void Record::AddOnePlace(){
	
#ifdef _DEBUG
	totalScore_ += 1;
#endif // _DEBUG

}

void Record::AddTenPlace(){
#ifdef _DEBUG
	totalScore_ += 10;
#endif // _DEBUG

}

void Record::AddHundredPlace(){
#ifdef _DEBUG
	totalScore_ += 100;
#endif // _DEBUG

}

void Record::AddThousandPlace(){
#ifdef _DEBUG
	totalScore_ += 1000;
#endif // _DEBUG

}

void Record::AddTenThousandPlace(){
#ifdef _DEBUG
	totalScore_ += 10000;
#endif // _DEBUG

}
