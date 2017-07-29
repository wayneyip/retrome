#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QPainter>
#include <QFont>

#include "mainapplication.h"
#include <iostream>

MainApplication::MainApplication(DataStore* ds) :
	ds_(ds)
{
	initializeComponents();
	setupLayout();
	styleLayout();
	connectEvents();

	this->setWindowTitle("RetroMe");
	this->setLayout(overallLayout);

	randomSelect();
}

MainApplication::~MainApplication()
{

}

void MainApplication::initializeComponents()
{
	overallLayout = new QVBoxLayout();

	// First row: title
	firstRowLayout = new QHBoxLayout();
	titleContainer = new QLabel();
	QImage title("img/title_1.png");
	// title = title.scaled(2 * title.size());
	titleContainer->setPixmap(QPixmap::fromImage(title));

	// Second row: avatar display + category/item selection
	secondRowLayout = new QHBoxLayout();

	// Second row left-side: avatar display
	avatarDisplayLayout = new QVBoxLayout();
	avatarContainer = new QLabel();
	
	// Second row right-upper: category selection
	selectionLayout = new QVBoxLayout();
	selectionCategoryTabMenu = new QTabWidget();
	selectionCategoryBodyList = new QListWidget();
	selectionCategoryClothingList = new QListWidget();
	selectionCategoryPropsList = new QListWidget();
	selectionCategoryBackdropList = new QListWidget();

	// Second row right-lower: item selection
	selectedCategoryLabel = new QLabel("");
	selectionItemList = new QListWidget();
	selectionColorList = new QListWidget();

	// Third row: random select + save + quit
	thirdRowLayout = new QHBoxLayout();
	randomSelectButton = new QPushButton("Random");
	saveButton = new QPushButton("Save Avatar");
	quitButton = new QPushButton("Quit");
}

void MainApplication::setupLayout()
{
	// First row: title
	overallLayout->addLayout(firstRowLayout);
	firstRowLayout->addWidget(titleContainer);

	// Second row: avatar display + category/item selection
	overallLayout->addLayout(secondRowLayout);

	// Second row left-side: avatar display
	secondRowLayout->addLayout(avatarDisplayLayout);
	avatarDisplayLayout->addWidget(avatarContainer);
	setupAvatar();

	// Second row right-upper: category selection
	secondRowLayout->addLayout(selectionLayout);
	selectionLayout->addWidget(selectionCategoryTabMenu);
	fillCategoryTabMenu();

	// Second row right-lower: item selection
	selectionLayout->addWidget(selectedCategoryLabel);
	selectionLayout->addWidget(selectionItemList);
	selectionItemList->setViewMode(QListWidget::IconMode);
	selectionItemList->setIconSize(QSize(200,200));
	selectionItemList->setResizeMode(QListWidget::Adjust);
	selectionItemList->setMovement(QListWidget::Static);
	selectionLayout->addWidget(selectionColorList);
	selectionColorList->setViewMode(QListWidget::IconMode);
	selectionColorList->setResizeMode(QListWidget::Adjust);
	selectionColorList->setMovement(QListWidget::Static);

	// Third row: random select + save + quit	
	overallLayout->addLayout(thirdRowLayout);
	thirdRowLayout->addWidget(randomSelectButton);
	thirdRowLayout->addWidget(saveButton);
	thirdRowLayout->addWidget(quitButton);
}

void MainApplication::fillCategoryTabMenu()
{
	selectionCategoryTabMenu->addTab(selectionCategoryBodyList, "Body");
	selectionCategoryBodyList->addItem("Body");
	selectionCategoryBodyList->addItem("Face");
	selectionCategoryBodyList->addItem("Hair");
	selectionCategoryBodyList->addItem("Eyebrows");
	selectionCategoryBodyList->addItem("Eyes");
	selectionCategoryBodyList->addItem("Nose");
	selectionCategoryBodyList->addItem("Mouth");
	selectionCategoryBodyList->addItem("Ears");	

	selectionCategoryTabMenu->addTab(selectionCategoryClothingList, "Clothing");
	selectionCategoryClothingList->addItem("Top");
	selectionCategoryClothingList->addItem("Bottom");
	selectionCategoryClothingList->addItem("Shoes");

	selectionCategoryTabMenu->addTab(selectionCategoryPropsList, "Props");
	selectionCategoryTabMenu->addTab(selectionCategoryBackdropList, "Backdrop");
}

void MainApplication::styleLayout()
{
	// Center app on screen
	QRect screenGeometry = QApplication::desktop()->screenGeometry();
	int x = (screenGeometry.width()-this->width()) / 2;
	int y = (screenGeometry.height()-this->height()) / 2;
	this->move(x, y);

	// Center title
	firstRowLayout->setAlignment(titleContainer, Qt::AlignCenter);

	// Resize color tab
	selectionColorList->setMaximumHeight(50);
}

void MainApplication::connectEvents()
{
	connect(selectionCategoryTabMenu, SIGNAL(currentChanged(int)), this, SLOT(changeType()));
	connect(selectionCategoryBodyList, SIGNAL(currentRowChanged(int)), this, SLOT(changeCategory()));
	connect(selectionCategoryClothingList, SIGNAL(currentRowChanged(int)), this, SLOT(changeCategory()));
	connect(selectionItemList, SIGNAL(currentRowChanged(int)), this, SLOT(selectItem()));
	connect(selectionColorList, SIGNAL(currentRowChanged(int)), this, SLOT(changeColor()));
	connect(randomSelectButton, SIGNAL(clicked()), this, SLOT(randomSelect()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveAvatar()));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));

}

void MainApplication::changeType()
{
	changeCategory();
}

void MainApplication::changeCategory()
{
	// Clear the item and color lists
	selectionItemList->clear();
	itemList_.clear();
	selectionColorList->clear();
	colorList_.clear();

	// Get currently active type
	QListWidget* categoryList = (QListWidget*)selectionCategoryTabMenu->currentWidget();

	// Do nothing if current type has no category selected 
	if (categoryList->currentItem() == NULL) return;

	// Update category label
	selectedCategoryLabel->setText(categoryList->currentItem()->text());
	
	// Get the item list for currently selected category
	std::string currentCategory
		= categoryList->item(categoryList->currentRow())->text().toStdString();

	itemList_ = ds_->getCategoryItems(currentCategory);

	// Display each item's icons
	for (unsigned int i=0; i < itemList_.size(); i++)
	{
		selectionItemList->addItem(
			new QListWidgetItem(QIcon(QString::fromStdString(itemList_[i]->getIconName())), NULL));
	}

	// Display the colors for currently selected category
	colorList_ = ds_->getCategoryColors(currentCategory);

	for (unsigned int i=0; i < colorList_.size(); i++)
	{
		QPixmap pixmap(AVATAR_SIZE/2);
		Color::shade mainColor = colorList_[i]->getMainColor();
		pixmap.fill(QColor(mainColor[0], mainColor[1], mainColor[2]));
		selectionColorList->addItem(new QListWidgetItem(QIcon(pixmap), NULL));
	}

	// Highlight the item in this category currently equipped by avatar, if any
	Item* equippedItem = ds_->findEquippedItem(currentCategory);
	if (equippedItem)
	{
		for (unsigned int i=0; i < itemList_.size(); i++)
		{
			if (itemList_[i] == equippedItem)
			{
				selectionItemList->setCurrentRow(i);
			}
		}
	}

	// Highlight the color in this category currently selected, if any
	Color* selectedColor = ds_->findSelectedColor(currentCategory);
	if (selectedColor)
	{
		for (unsigned int i=0; i < colorList_.size(); i++)
		{
			if (colorList_[i] == selectedColor)
			{
				selectionColorList->setCurrentRow(i);
			}
		}
	}
}

void MainApplication::selectItem()
{
  	// Do nothing if only the category was changed
	if (selectionItemList->currentRow() < 0) return;

	// If not, just select the clicked item
	ds_->selectItem(itemList_[selectionItemList->currentRow()]);
	updateAvatar();
}

void MainApplication::changeColor()
{
	// Do nothing if only the category was changed
	if (selectionColorList->currentRow() < 0) return;

	ds_->changeColor(colorList_[selectionColorList->currentRow()]);
	updateAvatar();
}

QPixmap MainApplication::setupAvatar()
{
	// Set up transparent pixmap
	getAvatarSize();
	QPixmap avatar(AVATAR_SIZE);
	avatar.fill(Qt::transparent);

	// Scale up avatar for display
	scaleAvatar(avatar);
	return avatar;
}

void MainApplication::getAvatarSize()
{
	QImage body("img/body_body_1.png");
	AVATAR_SIZE = body.size();
}

void MainApplication::scaleAvatar(QPixmap& avatar)
{
	QPixmap avatar_scaled;
	avatar_scaled.fill(Qt::transparent);
	avatar_scaled = avatar.scaled(2*AVATAR_SIZE, Qt::KeepAspectRatio);
	avatarContainer->setPixmap(avatar_scaled);
}

void MainApplication::updateAvatar()
{
	// Set up avatar's head and body
	QPixmap avatar = setupAvatar();

	// Paint each currently equipped item onto avatar
	DataStore::equippedItemHeap equippedItems = ds_->getAllEquippedItems();
	int equippedItemCount = equippedItems.size();
	for (int i=0; i < equippedItemCount; i++)
	{
		std::string equippedItem;
		
		// If item is layered...
		LayeredItem* lItem = dynamic_cast<LayeredItem*> (equippedItems.top());
		if (lItem && lItem->getLayer() == 1) // Item is in BACK state
		{
			// Store the item's backsprite
			equippedItem = lItem->getSpriteName();
			
			// Add the item's FRONT version to heap
			lItem->toggleLayer();
			equippedItems.push(lItem);
			equippedItemCount++;
		}
		// If item is not layered...
		else
		{
			// Just store the item's sprite
			equippedItem = equippedItems.top()->getSpriteName();
		}

		// Apply colors to item sprite, if any
		std::string category = equippedItems.top()->getCategory();
		Color* selectedColor = ds_->findSelectedColor(category);

		paintSprite(avatar, equippedItem, selectedColor);
		equippedItems.pop();
	}

	// Paint right hand onto avatar
	Color* skinColor = ds_->findSelectedColor("body");
	paintSprite(avatar, "img/body_hand_1.png", skinColor);

	// Scale up avatar for display
	scaleAvatar(avatar);
}

void MainApplication::paintSprite(QPixmap& avatar, std::string spriteName, Color* color)
{
	QPainter p(&avatar);
	QImage sprite(QString::fromStdString(spriteName));
	if (color)
	{
		std::vector<Color::shade> shades = color->getShades();
		for (unsigned int i=0; i < shades.size(); i++)
		{
			QColor shade(shades[i][0], shades[i][1], shades[i][2]);
			sprite.setColor(2 + i, shade.rgb());
		}
	}
	p.drawImage(QPoint(0, 0), sprite);
	p.end();
}

void MainApplication::randomSelect()
{
	ds_->selectRandomItems();
	updateAvatar();

	if (selectedCategoryLabel->text().isEmpty()) return;
	
	// Get the item list for currently selected category
	std::string currentCategory
		= selectedCategoryLabel->text().toStdString();

	itemList_ = ds_->getCategoryItems(currentCategory);

	// Highlight the item in this category currently equipped by avatar
	Item* equippedItem = ds_->findEquippedItem(currentCategory);
	
	for (unsigned int i=0; i < itemList_.size(); i++)
	{
		if (itemList_[i] == equippedItem)
		{
			selectionItemList->setCurrentRow(i);
		}
	}
}

void MainApplication::saveAvatar()
{
	// Ensure user provides filename in PNG
	QString filename = QFileDialog::getSaveFileName();
	if (! filename.endsWith(".png", Qt::CaseInsensitive))
	{
		filename = filename + ".png";
	}

	// Resize avatar pixmap (to regular size) and save
	QPixmap avatarToSave = avatarContainer->pixmap()
							->scaled(AVATAR_SIZE, Qt::KeepAspectRatio);
	avatarToSave.save(filename, "PNG");
}

void MainApplication::quit()
{
	QApplication::exit();
}

void MainApplication::PRINT(std::string statement)
{
	std::cout << statement << std::endl;
}