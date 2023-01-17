class PageEntry {
public:
	// Physical frame number for a given page
	int frameNum;
	// valid bit represents whether a page is in the physical memory
	bool valid = false;
	// dirty bit represents whether a page is changed
	bool dirty = false;

  PageEntry();
  PageEntry(int, bool);

  int getFrameNum();
  bool getValidBit();
  void setFrameNumber(int new_frame_number);
  void setValidBit(bool new_valid_bit);
};
