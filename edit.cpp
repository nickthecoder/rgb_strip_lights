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

void SpecialMode::drawScreen()
{
    Mode::drawScreen();
    u8g.drawStr( 0, 30, F("MODE: exit") );
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
    if ( controller.pCancelInput->keyPressed() ) {
        end();
    }
    
    // SEQUENCE BUTTON (Next Colour)
    if ( controller.pSequenceInput->keyPressed() ) {
        colorIndex ++;
        if ( colorIndex == controller.sequence.length ) {
            colorIndex = 0;
        }
        dvalue( "Next colour ", colorIndex );
    }
    
    // EDIT BUTTON (edit the current colour with the dials)
    if ( controller.pEditInput->keyPressed() ) {
        controller.setMode( &editMode );
        dprintln( "Edit" );
    }
    
    // ADD BUTTON (add a new colour with the dials)
    if ( controller.pAddInput->keyPressed() ) {
        controller.setMode( &addMode );
        dprintln( "Add" );
    }
    
    // DELETE BUTTON
    if ( controller.pDeleteInput->keyPressed() ) {
        if ( controller.sequence.length > 1 ) {
            controller.setMode( &deleteMode );
            dprintln( "Delete" );
        }
    }
    
    controller.color( controller.sequence.colors[ colorIndex ] );
}

void BrowseMode::drawScreen()
{
    SpecialMode::drawScreen();

    u8g.drawStr( 0, 46, F("SEQ: next colour") );    
    u8g.drawStr( 0, 62, F("EDIT/ADD/DEL") );    
}
  


EditMode editMode = EditMode();

void EditMode::display( float subTick )
{
    digitalWrite( EDIT_LED_PIN, subTick < 0.5 ? HIGH : LOW ); // Flash

    controller.color( controller.getDialColor() );

    if ( controller.pCancelInput->keyPressed() ) {
        controller.setMode( &browseMode );
        dprintln( "Back" );
    }
    
    if ( controller.pEditInput->keyPressed() ) {
        controller.sequence.setColor( browseMode.colorIndex, controller.getDialColor() );
        saveSequence( controller.sequenceIndex, &controller.sequence );
        dprintln( "Saved" );
        
        controller.setMode( &browseMode );
    }
}

void EditMode::drawScreen()
{
    SpecialMode::drawScreen();

    u8g.drawStr( 0, 46, F("Turn the dials!") );    
    u8g.drawStr( 0, 62, F("EDIT: save") );    
}
  


AddMode addMode = AddMode();

void AddMode::display( float subTick )
{
    digitalWrite( EDIT_LED_PIN, ( ((int) (subTick * 4)) % 2 == 0) ? HIGH : LOW ); // Double Flash

    controller.color( controller.getDialColor() );

    if ( controller.pCancelInput->keyPressed() ) {
        controller.setMode( &browseMode );
    }
    
    if ( controller.pAddInput->keyPressed() ) {
        controller.sequence.add( browseMode.colorIndex, controller.getDialColor() );
        saveSequence( controller.sequenceIndex, &controller.sequence );
        
        controller.setMode( &browseMode );
    }
}

void AddMode::drawScreen()
{
    SpecialMode::drawScreen();

    u8g.drawStr( 0, 46, F("Turn the dials!") );    
    u8g.drawStr( 0, 62, F("ADD: save") );    
}

DeleteMode deleteMode = DeleteMode();

void DeleteMode::display( float subTick )
{
    digitalWrite( EDIT_LED_PIN, ( ((int) (subTick * 6)) % 2 == 0) ? HIGH : LOW ); // Triple Flash

    if ( subTick < 0.5 ) {
      controller.tone( TONE_C6, 5 );
    }

    int part = subTick*10;
    controller.color( controller.sequence.colors[browseMode.colorIndex] );

    if ( controller.pCancelInput->keyPressed() ) {
        controller.setMode( &browseMode );
    }
    
    if ( controller.pDeleteInput->keyPressed() ) {
        controller.sequence.deleteColor( browseMode.colorIndex );
        if ( browseMode.colorIndex >= controller.sequence.length ) {
            browseMode.colorIndex = 0;
        }
        saveSequence( controller.sequenceIndex, &controller.sequence );
        
        controller.setMode( &browseMode );
    }
}

void DeleteMode::drawScreen()
{
    SpecialMode::drawScreen();

    u8g.drawStr( 0, 62, F("DEL: Delete") );
}


AddSequenceMode addSequenceMode = AddSequenceMode();

void AddSequenceMode::display( float subTick )
{
    digitalWrite( EDIT_LED_PIN, ( ((int) (subTick * 4)) % 2 == 0) ? HIGH : LOW ); // Double Flash

    controller.color( controller.getDialColor() );

    if ( controller.pCancelInput->keyPressed() ) {
        controller.setMode( controller.modeIndex );
    }
    
    if ( controller.pAddInput->keyPressed() ) {
        controller.sequence.clear();
        controller.sequence.append( controller.getDialColor() );
        saveSequence( sequenceCount(), &controller.sequence );
        
        browseMode.begin();
        controller.setMode( &browseMode );
    }

}

void AddSequenceMode::drawScreen()
{
    SpecialMode::drawScreen();

    u8g.drawStr( 0, 46, F("Turn the dials!") );
    u8g.drawStr( 0, 62, F("ADD: Add") );
}

DeleteSequenceMode deleteSequenceMode = DeleteSequenceMode();

void DeleteSequenceMode::display( float subTick )
{
    digitalWrite( EDIT_LED_PIN, ( ((int) (subTick * 6)) % 2 == 0) ? HIGH : LOW ); // Triple Flash

    if ( subTick < 0.5 ) {
      controller.tone( TONE_C6, 5 );
    }
    int part = subTick * controller.sequence.length;
    controller.color( controller.sequence.colors[part] );

    if ( controller.pCancelInput->keyPressed() ) {
        controller.setMode( controller.modeIndex );
        dprintln( "Back" );
    }
    
    if ( controller.pDeleteInput->keyPressed() ) {
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


void DeleteSequenceMode::drawScreen()
{
    SpecialMode::drawScreen();

    u8g.drawStr( 0, 62, F("DEL: Delete") );
}

