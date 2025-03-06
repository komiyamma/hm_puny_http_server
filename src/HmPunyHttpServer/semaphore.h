class HmSemaphore {
private:
	HANDLE hSemaphore;
public:
	HmSemaphore();
	~HmSemaphore();
	BOOL waitForOwnership();
};