#pragma once


#include <functional>

class TimedCall {
public:
	//コンストラクタ
	TimedCall(std::function<void()> func, uint32_t time);
	//更新
	void Update();
	//完了ならtrueを返す
	bool GetIsFinished() { return isFinished_;}

private:
	//コールバック
	std::function<void()> func_;
	//残り時間
	uint32_t time_;
	//完了フラグ
	bool isFinished_ = false;
};