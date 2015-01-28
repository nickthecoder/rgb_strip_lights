#ifndef HEADER_EDIT
#define HEADER_EDIT

class SpecialMode : public Mode
{
  public :
    virtual long getTickDuration();
    virtual void nextTick();

};

class BrowseMode : public SpecialMode
{
  public :
    int previousModeIndex;
    int colorIndex;

  public :
    virtual void begin();
    virtual void end();

    virtual void display( float subTicks );

};
extern BrowseMode browseMode;



class EditMode : public SpecialMode
{
  public :
    virtual void display( float subTicks );
};
extern EditMode editMode;


class AddMode : public SpecialMode
{
  public :
    virtual void display( float subTicks );
};
extern AddMode addMode;


class DeleteMode : public SpecialMode
{
  public :
    virtual void display( float subTicks );
};
extern DeleteMode deleteMode;



class AddSequenceMode : public SpecialMode
{
  public :
    virtual void display( float subTicks );
};
extern AddSequenceMode addSequenceMode;


class DeleteSequenceMode : public SpecialMode
{
  public :
    virtual void display( float subTicks );
};
extern DeleteSequenceMode deleteSequenceMode;



#endif
