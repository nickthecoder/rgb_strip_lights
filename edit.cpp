#include "controller.h"
#include "edit.h"

#define SPECIAL_DURATION 700;


long SpecialMode::getTickDuration()
{
    return SPECIAL_DURATION;
}


void SpecialMode::nextTick()
{
    // Do nothing!
}



BrowseMode browseMode = BrowseMode();

void BrowseMode::begin()
{
    SpecialMode::begin();

    dprintln( "Browse Begin" );
    digitalWrite( EDIT_LED_PIN, HIGH );
    previousModeIndex = controller.modeIndex;
    colorIndex = 0;
}

void BrowseMode::end()
{
    digitalWrite( EDIT_LED_PIN, LOW );
    controller.setMode( previousModeIndex );
    dprintln( "Browse End" );
}

void BrowseMode::display( float subTick )
{
    digitalWrite( EDIT_LED_PIN, HIGH );
    
    // MODE BUTTTON (exit browse, back to normal)
    if ( controller.modeButton.keyPressed() ) {
        end();
    }
    
    // SEQUENCE BUTTON (Next Colour)
    if ( controller.sequenceButton.keyPressed() ) {
        colorIndex ++;
        if ( colorIndex == controller.sequence.length ) {
            colorIndex = 0;
        }
        dvalue( "Next colour ", colorIndex );
    }
    
    // EDIT BUTTON (edit the current colour with the dials)
    if ( controller.editButton.keyPressed() ) {
        controller.setMode( &editMode );
        dprintln( "Edit" );
    }
    
    // ADD BUTTON (add a new colour with the dials)
    if ( controller.addButton.keyPressed() ) {
        controller.setMode( &addMode );
        dprintln( "Add" );
    }
    
    // DELETE BUTTON
    if ( controller.deleteButton.keyPressed() ) {
        if ( controller.sequence.length > 1 ) {
            controller.setMode( &deleteMode );
            dprintln( "Delete" );
        }
    }
    
    controller.color( controller.sequence.colors[ colorIndex ] );
}



EditMode editMode = EditMode();

void EditMode::display( float subTick )
{
    digitalWrite( EDIT_LED_PIN, subTick < 0.5 ? HIGH : LOW ); // Flash

    controller.color( controller.getDialColor() );

    if ( controller.modeButton.keyPressed() ) {
        controller.setMode( &browseMode );
        dprintln( "Back" );
    }
    
    if ( controller.editButton.keyPressed() ) {
        controller.sequence.setColor( browseMode.colorIndex, controller.getDialColor() );
        saveSequence( controller.sequenceIndex, &controller.sequence );
        dprintln( "Saved" );
        
        controller.setMode( &browseMode );
    }
}


AddMode addMode = AddMode();

void AddMode::display( float subTick )
{
    digitalWrite( EDIT_LED_PIN, ( ((int) (subTick * 4)) % 2 == 0) ? HIGH : LOW ); // Double Flash

    controller.color( controller.getDialColor() );

    if ( controller.modeButton.keyPressed() ) {
        controller.setMode( &browseMode );
        dprintln( "Back" );
    }
    
    if ( controller.addButton.keyPressed() ) {
        controller.sequence.add( browseMode.colorIndex, controller.getDialColor() );
        saveSequence( controller.sequenceIndex, &controller.sequence );
        dprintln( "Saved" );
        
        controller.setMode( &browseMode );
    }
}


DeleteMode deleteMode = DeleteMode();

void DeleteMode::display( float subTick )
{
    digitalWrite( EDIT_LED_PIN, ( ((int) (subTick * 6)) % 2 == 0) ? HIGH : LOW ); // Triple Flash

    int part = subTick*10;
    if ( (part == 1) || (part == 8) ) {
        controller.color( 255,0,0 );
    } else {
        controller.color( controller.sequence.colors[browseMode.colorIndex] );
    }

    if ( controller.modeButton.keyPressed() ) {
        controller.setMode( &browseMode );
        dprintln( "Back" );
    }
    
    if ( controller.deleteButton.keyPressed() ) {
        controller.sequence.deleteColor( browseMode.colorIndex );
        if ( browseMode.colorIndex >= controller.sequence.length ) {
            browseMode.colorIndex = 0;
        }
        saveSequence( controller.sequenceIndex, &controller.sequence );
        dprintln( "Saved" );
        
        controller.setMode( &browseMode );
    }
}


AddSequenceMode addSequenceMode = AddSequenceMode();

void AddSequenceMode::display( float subTick )
{
    digitalWrite( EDIT_LED_PIN, ( ((int) (subTick * 4)) % 2 == 0) ? HIGH : LOW ); // Double Flash

    controller.color( controller.getDialColor() );

    if ( controller.modeButton.keyPressed() ) {
        controller.setMode( controller.modeIndex );
        dprintln( "Back" );
    }
    
    if ( controller.addButton.keyPressed() ) {
        controller.sequence.clear();
        controller.sequence.append( controller.getDialColor() );
        saveSequence( sequenceCount(), &controller.sequence );
        dprintln( "Saved" );
        
        browseMode.begin();
        controller.setMode( &browseMode );
    }
}



DeleteSequenceMode deleteSequenceMode = DeleteSequenceMode();

void DeleteSequenceMode::display( float subTick )
{
    digitalWrite( EDIT_LED_PIN, ( ((int) (subTick * 6)) % 2 == 0) ? HIGH : LOW ); // Triple Flash

    if ( subTick < 0.5 ) {
      int part = subTick*8;
      if ( (part == 1) || (part == 3) ) {
          controller.color( 255,0,0 );
      } else {
          controller.color( 0,0,0 );
      }
    } else {

        int part = (subTick - 0.5) * controller.sequence.length;
        controller.color( controller.sequence.colors[part] );

    }

    if ( controller.modeButton.keyPressed() ) {
        controller.setMode( controller.modeIndex );
        dprintln( "Back" );
    }
    
    if ( controller.deleteButton.keyPressed() ) {
        deleteSequence( controller.sequenceIndex );
        if ( controller.sequenceIndex >= sequenceCount() ) {
            controller.setSequence( 0 );
        } else {
            controller.setSequence( controller.sequenceIndex );
        }
        
        digitalWrite( EDIT_LED_PIN, LOW ); // Edit LED off
        controller.setMode( controller.modeIndex );
    }
}



