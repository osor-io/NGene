#pragma once

/**
Curiously Recurring Template Pattern applyed to Singletons
*/
template <typename T>
class CRSP {
public:
	static T& get() {
		static T s_instance;
		return s_instance;
	}

protected:
	CRSP() {}
	virtual ~CRSP() {}

private:
	CRSP(const CRSP&) = delete;
	CRSP& operator=(const CRSP&) = delete;

	CRSP(const CRSP&&) = delete;
	CRSP& operator=(const CRSP&&) = delete;
};