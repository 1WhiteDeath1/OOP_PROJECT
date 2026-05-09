#pragma once
class PlayerSoldier;

class TransformativeState
{
public:
	virtual void apply(PlayerSoldier* soldier, float frameTime) = 0;

	virtual bool canUseWeapon() const = 0;
	virtual bool canUseMelee() const = 0;
	virtual float getSpeedMultiplier() const = 0;
	virtual int getType() const = 0;
	virtual ~TransformativeState() = default;
};

class NormalState : public TransformativeState {
public:
	void apply(PlayerSoldier* soldier, float frameTime) override {};
	bool canUseWeapon()const override{
		return true;
	}
	bool canUseMelee() const override {
		return true;
	}
	float getSpeedMultiplier() const override {
		return 1;
	}
	int getType() const override {
		return 0;
	}
};

class UndeadState : public TransformativeState {
public: 
	void apply(PlayerSoldier* soldier, float frameTime) override {}
	bool  canUseWeapon()        const override {
		return true; }
	bool  canUseMelee()         const override {
		return true; }
	float getSpeedMultiplier()  const override {
		return 0.5f; } // 50% speed
	int   getType()             const override {
		return 1; }
};


class MummyState : public TransformativeState
{
public:
	void  apply(PlayerSoldier* soldier, float dt) override {}
	bool  canUseWeapon()        const override {
		return false; } // no guns
	bool  canUseMelee()         const override {
		return true; } // knife only
	float getSpeedMultiplier()  const override { 
		return 1.0f; } // no speed change
	int   getType()             const override { 
		return 2; }
};

