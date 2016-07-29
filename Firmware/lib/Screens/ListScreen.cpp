#include "ListScreen.h"

ListScreen::ListScreen(SSD1306 *display)
    : Screen(display), mItems(new LinkedList<ListItem *>()), mScrollOffset(0),
      mSelectedElementOnScreen(0) {}

ListScreen::~ListScreen() {
  mItems->clear();
  delete mItems;
}

void ListScreen::draw() {
  this->clear();
  mDisplay->setFont(ArialMT_Plain_10);
  mDisplay->setColor(WHITE);
  for (int i = 0; i < ITEMS_ON_SCREEN; i++) {
    ListItem *item = mItems->at(i + mScrollOffset);
    if (item != NULL) {

      // the extra always should just be a short slug
      // so it always is in full length and the name gets the rest of the place
      int extraLength = mDisplay->getStringWidth(item->getExtra()) +
                        INDICATOR_WIDTH + PADDING;

      String shortenedName =
          this->trimStringToLength(item->getName(), SCREEN_WIDTH - extraLength);

      int elementYPos = SCREEN_START_Y + ((i * ITEM_HEIGHT) - 1) + LIST_OFFSET;
      int nameXPos = SCREEN_START_X;
      int extraXPos = SCREEN_WIDTH - INDICATOR_WIDTH;
      int indicatorXPos = SCREEN_WIDTH;

      // draw the indicator
      if (item->isClickable()) {
        mDisplay->setTextAlignment(TEXT_ALIGN_RIGHT);
        mDisplay->drawString(indicatorXPos, elementYPos, ELEMENT_INDICATOR);
      }

      // draw the extra
      mDisplay->setTextAlignment(TEXT_ALIGN_RIGHT);
      mDisplay->drawString(extraXPos, elementYPos, item->getExtra());

      // draw the shortened name
      mDisplay->setTextAlignment(TEXT_ALIGN_LEFT);
      mDisplay->drawString(nameXPos, elementYPos, shortenedName);
    }
  }

  mDisplay->setColor(INVERSE);
  mDisplay->fillRect(SCREEN_START_X,
                     SCREEN_START_Y + (mSelectedElementOnScreen * ITEM_HEIGHT) +
                         LIST_OFFSET,
                     SCREEN_WIDTH, ITEM_HEIGHT);
}

bool ListScreen::cursorDown(int steps) {
  mSelectedElementOnScreen += steps;
  if (mSelectedElementOnScreen >= ITEMS_ON_SCREEN) {
    mScrollOffset += mSelectedElementOnScreen - (ITEMS_ON_SCREEN - 1);
    mSelectedElementOnScreen = ITEMS_ON_SCREEN - 1;
  }

  if ((mScrollOffset + mSelectedElementOnScreen) > mItems->size()) {
    mSelectedElementOnScreen = ITEMS_ON_SCREEN - 1;
    mScrollOffset = mItems->size() - mSelectedElementOnScreen;
  }
}

bool ListScreen::cursorUp(int steps) {
  mSelectedElementOnScreen -= steps;
  if (mSelectedElementOnScreen < 0) {
    mScrollOffset += mSelectedElementOnScreen;
    mSelectedElementOnScreen = 0;
  }

  if (mScrollOffset < 0) {
    mScrollOffset = 0;
    mSelectedElementOnScreen = 0;
  }
}
