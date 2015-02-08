#ifndef HEADER_EDIT
#define HEADER_EDIT

class SpecialMode : public Mode
{
  public :
    SpecialMode( const __FlashStringHelper *name ) : Mode( name ) {};

    virtual long getTickDuration();
    virtual void nextTick();

};

class BrowseMode : public SpecialMode
{
  public :
    BrowseMode() : SpecialMode( F("Edit Sequence") ) {};
    
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
    EditMode() : SpecialMode( F("Edit Colour") ) {};

    virtual void display( float subTicks );
};
extern EditMode editMode;


class AddMode : public SpecialMode
{
  public :
    AddMode() : SpecialMode( F("Add Colour") ) {};

    virtual void display( float subTicks );
};
extern AddMode addMode;


class DeleteMode : public SpecialMode
{
  public :
    DeleteMode() : SpecialMode( F("Delete Colour") ) {};

    virtual void display( float subTicks );
};
extern DeleteMode deleteMode;



class AddSequenceMode : public SpecialMode
{
  public :
    AddSequenceMode() : SpecialMode( F("Add Sequence") ) {};

    virtual void display( float subTicks );
};
extern AddSequenceMode addSequenceMode;


class DeleteSequenceMode : public SpecialMode
{
  public :
    DeleteSequenceMode() : SpecialMode( F("Delete Sequence") ) {};

    virtual void display( float subTicks );
};
extern DeleteSequenceMode deleteSequenceMode;



#endif

