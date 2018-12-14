/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectFactory.cpp
Purpose: Object Factory
Language: C++ language
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: Game Engine
Author: Fenil Shingala, fenil.shingala, 60003118
Creation date: 12/07/2018
- End Header --------------------------------------------------------*/

#include "ObjectFactory.h"

#include "Managers/GameStateManager.h"
#include "Managers/InputManager.h"
#include <irrKlang.h>
using namespace irrklang;

#include <ctime>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

using namespace rapidjson;

extern GameObject* pObject1;
extern GameObjectManager *pGameObjectManager;
extern ResourceManager* pResourceManager;
extern GameStateManager* pGameStateManager;
extern InputManager* pInputManager;
extern ISoundEngine *SoundEngine;

extern int width;
extern int height;

void ObjectFactory::initializeMembers()
{
	mNoOfEnemyShip = mNoOfMeteors = mNoOfStars = meteorsDestroyed = meteorsAlive = totalMeteors = 0;
	isBulletAllowed = isNuclearAllowed = isHomingAllowed = isEnemnyShipAllowed = false;
}

void ObjectFactory::update()
{
	if (totalMeteors <= 0 && mNoOfMeteors != 0 && currentLevel != "Level3.json") {
		std::cout << "CHANGE LEVEL\n";
		auto itr = std::find(GameLevels.begin(), GameLevels.end(), currentLevel);
		if (++itr != GameLevels.end())
			levelPool = *itr;
		else
			levelPool = "GameOver.json";
	}

	if (win) { levelPool = "Win.json"; win = false; }

	if ("" != levelPool) {
		unsigned int size = pGameObjectManager->mGameObjects.size();
		for(unsigned int i=0; i<size; ++i)
			deleteObjects.push_back(pGameObjectManager->mGameObjects[i]);
	}

	if (false == deleteObjects.empty()) {
		unsigned int count = pGameObjectManager->mGameObjects.size();
		for (auto obj : deleteObjects) {

			for (unsigned int i = 0; i < count; ++i) {
				if (pGameObjectManager->mGameObjects[i] == obj) {
					obj->mFlag = false;
					if (pGameObjectManager->mGameObjects[i]->mType == METEOR) {
						--meteorsAlive;
						--totalMeteors;
					}
					break;
				}
			}
		}
		deleteObjects.clear();
	}

	if ("" != levelPool) {
		initializeMembers();
		LoadLevel(levelPool.c_str());
		currentLevel = levelPool;
		levelPool = "";
	}

	if (false == newObjects.empty()) {
		for (auto obj : newObjects) {
			unsigned int size = pGameObjectManager->mGameObjects.size();
			for (unsigned int i = 0; i < size; ++i) {
				
				if (pGameObjectManager->mGameObjects[i]->mFlag == false) {
					pGameObjectManager->mGameObjects[i] = obj;
					obj->mFlag = true;
					break;
				}
			
			}
			if (false == obj->mFlag) {
				pGameObjectManager->mGameObjects.push_back(obj);
				obj->mFlag = true;
			}
		}
		newObjects.clear();
	}
	
	if (meteorsAlive < 2 && mNoOfMeteors != 0 && !isEnemyShipAlive) {
		std::cout << "LESSS!!!!!!!!!!\n";
		createMeteors();
	}
	if(isEnemnyShipAllowed)
		if (totalMeteors <= 0)
			spawnEnemyShip();
}

void ObjectFactory::LoadLevelMeta(const char* pFileName)
{
	FILE *fp;
	std::string fullPath = "../Resources/Levels/";
	fullPath += pFileName;
	fopen_s(&fp, fullPath.c_str(), "r");

	char buffer[256] = { 0 };
	FileReadStream stream(fp, buffer, sizeof(buffer));
	Document json;
	json.ParseStream(stream);

	if (json.HasMember("Initiate")) {
		std::string init = json["Initiate"].GetString();
		pGameStateManager->setInitialLevel(init.c_str());
	}
	else
		assert("Initial Level not found");


	if (fp) {
		const Value& a = json["Levels"];
		for (unsigned int i = 0; i < a.Size(); i++) {
			std::string levelFile = a[i].GetString();

			FILE *fptr;
			std::string fullLevelPath = "../Resources/Levels/";
			fullLevelPath += levelFile;
			fopen_s(&fptr, fullLevelPath.c_str(), "r");

			char buffer[256] = { 0 };
			FileReadStream stream(fptr, buffer, sizeof(buffer));
			Document* doc = new Document;
			doc->ParseStream(stream);

			if (fptr) {
				mLevelData[levelFile] = doc;
			}
			fclose(fptr);
		}

		const Value& b = json["GameLevels"];
		for (unsigned int i = 0; i < b.Size(); i++) {
			GameLevels.push_back(b[i].GetString());
		}

	}
	fclose(fp);
}

void ObjectFactory::LoadLevel(const char* pFileName)
{
	Document* jsonObj = new Document;

	auto itr = mLevelData.find(pFileName);
	//Already Loaded?
	if (itr != mLevelData.end())
		jsonObj = itr->second;
	else
		return;
	
	GenericObject<false, Value::ValueType> json = jsonObj->GetObject();

	// LOAD PREFABS
	if (json.HasMember("ListOfObjectFiles")) {
		auto objectFiles = json["ListOfObjectFiles"].GetArray();
		unsigned int size = objectFiles.Size();
		for (unsigned int i = 0; i < size; ++i) {
			std::string file = objectFiles[i].GetString();
			LoadObjectPrefab(file.c_str());
		}
	}

	if (json.HasMember("noOfEnemyShip"))
		mNoOfEnemyShip = (unsigned int)json["noOfEnemyShip"].GetInt();
	if (json.HasMember("noOfMeteors"))
		mNoOfMeteors = (unsigned int)json["noOfMeteors"].GetInt();
	if (json.HasMember("noOfHoming"))
		mNoOfHoming = (unsigned int)json["noOfHoming"].GetInt();
	if (json.HasMember("noOfNuclear"))
		mNoOfNuclear = (unsigned int)json["noOfNuclear"].GetInt();
	if (json.HasMember("TotalMeteors"))
		totalMeteors = (unsigned int)json["TotalMeteors"].GetInt();
	if (json.HasMember("MeteorVelocity"))
		meteorVelocity = (float)json["MeteorVelocity"].GetInt();
	
	if (json.HasMember("noOfStars"))
		mNoOfStars = (unsigned int)json["noOfStars"].GetInt();
	createStars();

	if (json.HasMember("BulletAllowed"))
		isBulletAllowed = json["BulletAllowed"].GetBool();
	if (json.HasMember("HomingAllowed"))
		isHomingAllowed = json["HomingAllowed"].GetBool();
	if (json.HasMember("NuclearAllowed"))
		isNuclearAllowed = json["NuclearAllowed"].GetBool();
	if (json.HasMember("EnemnyShipAllowed"))
		isEnemnyShipAllowed = json["EnemnyShipAllowed"].GetBool();

	GameObject* pGameObj = new GameObject;

	// LOAD PLAYERS IN LEVELS
	for (Value::MemberIterator objItr = json.MemberBegin(); objItr != json.MemberEnd(); ++objItr)
	{
		if (objItr->value.IsObject()){
			rapidjson::GenericObject<false, rapidjson::Value::ValueType> obj = objItr->value.GetObject();

			if (obj.HasMember("file")) {
				if (obj["file"].IsString()) {
					std::string fileName = obj["file"].GetString();
					if (fileName == "Meteor.json") {
						continue;
					}
					if (fileName == "Stars.json") {
						continue;
					}

					pGameObj = LoadObject(obj["file"].GetString());
					if (nullptr == pGameObj) {
						assert("Prefab not found!");
						return;
					}
				}
			}

			// OVERRIDE COMPONENTS
			/* TRANSFORM */
			if (obj.HasMember("Transform")) {
				if (obj["Transform"].IsObject()) {
					Transform *pTr = static_cast<Transform*>(pGameObj->GetComponent(TRANSFORM));
					if (nullptr != pTr)
						pTr->Serialize(obj["Transform"].GetObject());
					else {
						pTr = static_cast<Transform*>(pGameObj->AddComponent(TRANSFORM));
						pTr->Serialize(obj["Transform"].GetObject());
					}

					Body *pBody = static_cast<Body*>(pGameObj->GetComponent(BODY));
					if (nullptr != pBody) {
						pBody->Initialize();
					}
				}
			}

			/* SPRITE */
			if (obj.HasMember("Sprite")) {
				if (obj["Sprite"].IsObject()) {
					Sprite *pSpr = static_cast<Sprite*>(pGameObj->GetComponent(SPRITE));
					if (nullptr != pSpr)
						pSpr->Serialize(obj["Sprite"].GetObject());
					else {
						pSpr = static_cast<Sprite*>(pGameObj->AddComponent(SPRITE));
						pSpr->Serialize(obj["Sprite"].GetObject());
					}
				}
			}

			/* BODY */
			if (obj.HasMember("Body")) {
				if (obj["Body"].IsObject()) {
					Body *pBody = static_cast<Body*>(pGameObj->GetComponent(BODY));
					if (nullptr != pBody) {
						pBody->Serialize(obj["Body"].GetObject());
						pBody->Initialize();
					}
					else {
						pBody = static_cast<Body*>(pGameObj->AddComponent(BODY));
						pBody->Serialize(obj["Body"].GetObject());
					}
				}
			}

			/* CONTROLLER */
			if (obj.HasMember("Controller")) {
				if (obj["Controller"].IsObject()) {
					Controller *pCtrl = static_cast<Controller*>(pGameObj->GetComponent(CONTROLLER));
					if (nullptr != pCtrl)
						pCtrl->Serialize(obj["Controller"].GetObject());
					else {
						pCtrl = static_cast<Controller*>(pGameObj->AddComponent(CONTROLLER));
						pCtrl->Serialize(obj["Controller"].GetObject());
					}
				}
			}

			/* UPDOWN */
			if (obj.HasMember("UpDown")) {
				if (obj["UpDown"].IsObject()) {
					UpDown *pUpDown = static_cast<UpDown*>(pGameObj->GetComponent(UPDOWN));
					if (nullptr != pUpDown)
						pUpDown->Serialize(obj["UpDown"].GetObject());
					else {
						pUpDown = static_cast<UpDown*>(pGameObj->AddComponent(UPDOWN));
						pUpDown->Serialize(obj["UpDown"].GetObject());
					}
				}
			}

			/* BUTTON */
			if (obj.HasMember("Button")) {
				if (obj["Button"].IsObject()) {
					Button *pButton = static_cast<Button*>(pGameObj->GetComponent(BUTTON));
					if (nullptr != pButton)
						pButton->Serialize(obj["Button"].GetObject());
					else {
						pButton = static_cast<Button*>(pGameObj->AddComponent(BUTTON));
						pButton->Serialize(obj["Button"].GetObject());
					}
				}
			}

			/* TARGET */
			if (obj.HasMember("Target")) {
				if (obj["Target"].IsObject()) {
					Target *pTarget = static_cast<Target*>(pGameObj->GetComponent(TARGET));
					if (nullptr != pTarget)
						pTarget->Serialize(obj["Target"].GetObject());
					else {
						pTarget = static_cast<Target*>(pGameObj->AddComponent(TARGET));
						pTarget->Serialize(obj["Button"].GetObject());
					}
				}
			}

			/* AI */
			if (obj.HasMember("AI")) {
				if (obj["AI"].IsObject()) {
					AI *pAI = static_cast<AI*>(pGameObj->GetComponent(OBJAI));
					if (nullptr != pAI)
						pAI->Serialize(obj["AI"].GetObject());
					else {
						pAI = static_cast<AI*>(pGameObj->AddComponent(OBJAI));
						pAI->Serialize(obj["AI"].GetObject());
					}
				}
			}

			/* Text */
			if (obj.HasMember("Text")) {
				if (obj["Text"].IsObject()) {
					Text *pText = static_cast<Text*>(pGameObj->GetComponent(TEXT));
					if (nullptr != pText)
						pText->Serialize(obj["Text"].GetObject());
					else {
						pText = static_cast<Text*>(pGameObj->AddComponent(TEXT));
						pText->Serialize(obj["Text"].GetObject());
					}
				}
			}

			/* Expand */
			if (obj.HasMember("Expand")) {
				if (obj["Expand"].IsObject()) {
					Expand *pExpand = static_cast<Expand*>(pGameObj->GetComponent(EXPAND));
					if (nullptr != pExpand)
						pExpand->Serialize(obj["Expand"].GetObject());
					else {
						pExpand = static_cast<Expand*>(pGameObj->AddComponent(EXPAND));
						pExpand->Serialize(obj["Expand"].GetObject());
					}
				}
			}

		}
	}

	// END LOADING PLAYERS IN LEVELS
	createMeteors();
}

void ObjectFactory::LoadObjectPrefab(const char *pFileName) {
	std::string fullPath = "../Resources/Levels/";
	fullPath += pFileName;
	Document* json;

	auto itr = mPrefabs.find(fullPath);
	//Already Loaded?
	if (itr != mPrefabs.end())
		json = itr->second;

	else {
		FILE *fp;
		json = new Document;
		fopen_s(&fp, fullPath.c_str(), "r");

		char buffer[256] = { 0 };

		//pNewGameObject->id = ++ObjectFactory::objectCount;
		if (fp) {
			FileReadStream stream(fp, buffer, sizeof(buffer));
			json->ParseStream(stream);
			mPrefabs[fullPath] = json;
		}
		fclose(fp);
	}
}

GameObject* ObjectFactory::LoadObject(const char *pFileName)
{
	std::string fullPath = "../Resources/Levels/";
	fullPath += pFileName;
	
	Document* json;

	auto itr = mPrefabs.find(fullPath);
	//Already Loaded?
	if (itr != mPrefabs.end())
		json = itr->second;
	else
		return nullptr;

	GenericObject<false, Value::ValueType> jsonObj = json->GetObject();
	GameObject* pNewGameObject = new GameObject();
	Component *pNewComponent = nullptr;

	if (jsonObj.HasMember("Type")) {
		std::string Type = jsonObj["Type"].GetString();
		if ("Player" == Type) {
			pNewGameObject->mType = PLAYER;
			pGameObjectManager->mPlayer = pNewGameObject;
		}
		else if ("Meteor" == Type) {
			pNewGameObject->mType = METEOR;
		}
		else if ("Bullet" == Type) {
			pNewGameObject->mType = BULLET;
		}
		else if ("RectButton" == Type) {
			pNewGameObject->mType = RECT_BUTTON;
		}
		else if ("Stars" == Type) {
			pNewGameObject->mType = STAR;
		}
		else if ("HomingMissile" == Type) {
			pNewGameObject->mType = HOMINGMISSILE;
		}
		else if ("NuclearMissile" == Type) {
			pNewGameObject->mType = NUCLEARMISSILE;
		}
		else if ("EnemyShip" == Type) {
			pNewGameObject->mType = ENEMYSHIP;
		}
		else if ("Text" == Type) {
			pNewGameObject->mType = TEXTOBJ;
		}
		else if ("Shield" == Type) {
			pNewGameObject->mType = SHIELD;
		}
	}

	if (jsonObj.HasMember("Transform")) {
		pNewComponent = pNewGameObject->AddComponent(TRANSFORM);
		if (nullptr != pNewComponent)
			pNewComponent->Serialize(jsonObj["Transform"].GetObject());
	}

	if (jsonObj.HasMember("Sprite")) {
		pNewComponent = pNewGameObject->AddComponent(SPRITE);
		if (nullptr != pNewComponent)
			pNewComponent->Serialize(jsonObj["Sprite"].GetObject());
	}

	if (jsonObj.HasMember("Body")) {
		pNewComponent = pNewGameObject->AddComponent(BODY);
		if (nullptr != pNewComponent) {
			pNewComponent->Serialize(jsonObj["Body"].GetObject());
			Body *pBody = static_cast<Body*>(pNewGameObject->GetComponent(BODY));
			pBody->Initialize();
		}
	}

	if (jsonObj.HasMember("Controller")) {
		pNewComponent = pNewGameObject->AddComponent(CONTROLLER);
	}

	if (jsonObj.HasMember("UpDown")) {
		pNewComponent = pNewGameObject->AddComponent(UPDOWN);
		if (nullptr != pNewComponent)
			pNewComponent->Serialize(jsonObj["UpDown"].GetObject());
	}

	if (jsonObj.HasMember("Up")) {
		pNewComponent = pNewGameObject->AddComponent(UP);
		if (nullptr != pNewComponent) {
			pNewComponent->Serialize(jsonObj["Up"].GetObject());
		}
	}

	if (jsonObj.HasMember("Button")) {
		pNewComponent = pNewGameObject->AddComponent(BUTTON);
		if (nullptr != pNewComponent) {
			pNewComponent->Serialize(jsonObj["Button"].GetObject());
		}
	}

	if (jsonObj.HasMember("Target")) {
		pNewComponent = pNewGameObject->AddComponent(TARGET);
		if (nullptr != pNewComponent) {
			pNewComponent->Serialize(jsonObj["Target"].GetObject());
		}
	}

	if (jsonObj.HasMember("Target")) {
		pNewComponent = pNewGameObject->AddComponent(TARGET);
		if (nullptr != pNewComponent) {
			pNewComponent->Serialize(jsonObj["Target"].GetObject());
		}
	}

	if (jsonObj.HasMember("AI")) {
		pNewComponent = pNewGameObject->AddComponent(OBJAI);
		if (nullptr != pNewComponent) {
			pNewComponent->Serialize(jsonObj["AI"].GetObject());
		}
	}

	if (jsonObj.HasMember("Text")) {
		pNewComponent = pNewGameObject->AddComponent(TEXT);
		if (nullptr != pNewComponent) {
			pNewComponent->Serialize(jsonObj["Text"].GetObject());
		}
	}

	if (jsonObj.HasMember("Expand")) {
		pNewComponent = pNewGameObject->AddComponent(EXPAND);
		if (nullptr != pNewComponent) {
			pNewComponent->Serialize(jsonObj["Expand"].GetObject());
		}
	}
	
	newObjects.push_back(pNewGameObject);

	return pNewGameObject;
}

void ObjectFactory::initiateLevel(const char* initialLevel)
{
	currentLevel = initialLevel;
	LoadLevel(initialLevel);
}

void ObjectFactory::changeLevel(const char* levelName) { levelPool = levelName; }

void ObjectFactory::nextLevel() {
	auto itr = std::find(GameLevels.begin(), GameLevels.end(), currentLevel);
	if (++itr != GameLevels.end())
		levelPool = *itr;
	else
		std::cout << "No next Level!";
}

void ObjectFactory::createBullet(float x, float y, unsigned int type, GameObject* pPlayer)
{
	if (type == NUCLEARMISSILE && mNoOfNuclear <= 0)
		return;
	else if(type == NUCLEARMISSILE && mNoOfNuclear>0) --mNoOfNuclear;
		
	GameObject* newObject = new GameObject();
	GameObject* pPl = pGameObjectManager->mPlayer;

	if (type == BULLET) {
		if (!isBulletAllowed)
			return;
		newObject = LoadObject("Bullet.json");
	}
	else if (type == NUCLEARMISSILE) {
		if (!isNuclearAllowed)
			return;
		newObject = LoadObject("NuclearMissile.json");
	}

	Transform* pTr = static_cast<Transform*>(newObject->GetComponent(TRANSFORM));
	Transform* pPlTr = static_cast<Transform*>(pPl->GetComponent(TRANSFORM));

	if (nullptr != pTr) {
		pTr->mPosX = x;

		float shiftY = 0.0f;
		if (nullptr != pPl)
			shiftY = pPlTr->scaleY / 2.0f;
		pTr->mPosY = y+shiftY;

		if (nullptr == pPlayer) {
			int mouseX = 0, mouseY = 1;

			if ( type == BULLET && pInputManager->isButtonTriggerred(SDL_BUTTON_LEFT) ||
				 type == NUCLEARMISSILE && pInputManager->isButtonTriggerred(SDL_BUTTON_RIGHT)
			   )
			{
				SDL_GetMouseState(&mouseX, &mouseY);
				float mAngleX = (float)mouseX - x;
				float mAngleY = height - (float)mouseY - y;
				pTr->mAngle = atan2f(mAngleY, mAngleX);
			}

			Body *pBody = static_cast<Body*>(newObject->GetComponent(BODY));
			if (nullptr != pBody) {
				pBody->Initialize();
				pBody->mVelX = cosf(pTr->mAngle) * 400000.0f;
				pBody->mVelY = sinf(pTr->mAngle) * 400000.0f;
			}
			if (type == BULLET)
				SoundEngine->play2D("../Resources/Audio/lasers/7.wav", GL_FALSE);
		}
		else {
			newObject->mType = ENEMYBULLET;
			Transform* playerTr = static_cast<Transform*>(pPlayer->GetComponent(TRANSFORM));
			float mAngleX = playerTr->mPosX - x;
			float mAngleY = playerTr->mPosY - y;
			pTr->mAngle = atan2f(mAngleY, mAngleX);

			Body *pBody = static_cast<Body*>(newObject->GetComponent(BODY));
			if (nullptr != pBody) {
				pBody->Initialize();
				pBody->mVelX = cosf(pTr->mAngle) * 400000.0f;
				pBody->mVelY = sinf(pTr->mAngle) * 400000.0f;
			}
		}
	}
}

void ObjectFactory::createObjectAtShip(std::string file)
{
	if (file == "HomingMissile.json" && mNoOfHoming <= 0)
		return;
	else if (file == "HomingMissile.json" && mNoOfHoming > 0) --mNoOfHoming;

	if ("HomingMissile.json" == file) {
		if (!isHomingAllowed)
			return;
	}
	GameObject* newObject = new GameObject();
	GameObject* pPl = pGameObjectManager->mPlayer;

	newObject = LoadObject(file.c_str());
	
	Transform* pTr = static_cast<Transform*>(newObject->GetComponent(TRANSFORM));
	Transform* pPlTr = static_cast<Transform*>(pPl->GetComponent(TRANSFORM));

	float x = pPlTr->mPosX;
	float y = pPlTr->mPosY;

	pTr->mPosX = x;
	float shiftY = 0.0f;

	if ("HomingMissile.json" == file) {
		if (nullptr != pPl)
			shiftY = pPlTr->scaleY / 2.0f;
	}

	pTr->mPosY = y + shiftY;

	Body *pBody = static_cast<Body*>(newObject->GetComponent(BODY));
	if (nullptr != pBody) {
		pBody->Initialize();

		if ("HomingMissile.json" == file) {
			pTr->mAngle += (float)(PI / 2.0f);
			pBody->mVelX = cosf(pTr->mAngle) * 401000.0f;
			pBody->mVelY = sinf(pTr->mAngle) * 401000.0f;
		}
		else if ("Shield.json") {
			pBody->mVelX = 0.0f;
			pBody->mVelY = 0.0f;
			pTr->scaleX = pPlTr->scaleX * 1.5f;
			pTr->scaleY = pPlTr->scaleY * 1.5f;
			pBody->Initialize();
		}
	}
}

void ObjectFactory::createMeteors()
{
	std::srand((unsigned int)std::time(nullptr));
	unsigned int n = mNoOfMeteors;
	while (0 != n) {
		GameObject* newObject = new GameObject();
		newObject = LoadObject("Meteor.json");
		Transform* pTr = static_cast<Transform*>(newObject->GetComponent(TRANSFORM));
		if(nullptr == pTr)
			pTr = static_cast<Transform*>(newObject->AddComponent(TRANSFORM));
		if (nullptr != pTr) {
			pTr->mPosX = 100.0f + (float)(std::rand() % 600);
			pTr->mPosY = (float)(height + (std::rand() % ((int)pTr->scaleY*40 + 1)));
		}
		Body *pBody = static_cast<Body*>(newObject->GetComponent(BODY));
		if (nullptr != pBody) {
			pBody->Initialize();
			pBody->mVelY = -meteorVelocity;
		}
		--n;
	}
	meteorsAlive += mNoOfMeteors;
}

void ObjectFactory::createStars()
{
	std::srand((unsigned int)std::time(nullptr));
	while (0 != mNoOfStars) {
		GameObject* newObject = new GameObject();
		newObject = LoadObject("Stars.json");
		Transform* pTr = static_cast<Transform*>(newObject->GetComponent(TRANSFORM));
		if(nullptr == pTr)
			pTr = static_cast<Transform*>(newObject->AddComponent(TRANSFORM));
		if (nullptr != pTr) {
			pTr->mPosX = (float)(0 + (std::rand() % (width)) + 1);
			pTr->mPosY = (float)(0 + (std::rand() % (height)) + 1);
		}

		--mNoOfStars;
	}
}

void ObjectFactory::deleteObject(GameObject* pGameObject)
{
	Sprite* pSpr = static_cast<Sprite*>(pGameObject->GetComponent(SPRITE));
	if (nullptr != pSpr && true == pSpr->hasAnim) {
		pSpr->isAnim = true;
		pSpr->mTextureID = pSpr->mAnimeID;
		pSpr->currentTextureOffset = pSpr->animeTextureOffset;
	}
	else { deleteObjects.push_back(pGameObject); }
}

void ObjectFactory::spawnEnemyShip()
{
	std::srand((unsigned int)std::time(nullptr));
	if (mNoOfEnemyShip > 0) isEnemyShipAlive = true;
	while (0 != mNoOfEnemyShip) {
		GameObject* newObject = new GameObject();
		newObject = LoadObject("EnemyShip.json");
		Transform* pTr = static_cast<Transform*>(newObject->GetComponent(TRANSFORM));
		if (nullptr == pTr)
			pTr = static_cast<Transform*>(newObject->AddComponent(TRANSFORM));
		if (nullptr != pTr) {
			float marginX = 2 * pTr->scaleX;
			float marginY = pTr->scaleY;
			pTr->mPosX = (float)( marginX + (std::rand() % (int)(width-2*marginX)) + 1 );
			pTr->mPosY = (float)( (height+marginY) + (std::rand() % (int)marginY) + 1 );
		}
		Body *pBody = static_cast<Body*>(newObject->GetComponent(BODY));
		if (nullptr != pBody) {
			pBody->Initialize();
			pBody->mVelX = 0.0f;
			pBody->mVelY = -100000.0f;
		}
		--mNoOfEnemyShip;
	}
}

void ObjectFactory::destroyPause()
{
	unsigned int size = pGameObjectManager->mGameObjects.size();
	for (unsigned int i = 0; i < size; ++i)
	{
		Button *pButton = static_cast<Button*>(pGameObjectManager->mGameObjects[i]->GetComponent(BUTTON));
		if (nullptr != pButton) {
			if (pButton->mGameMode == PAUSE_CONTINUE ||
				pButton->mGameMode == PAUSE_QUIT	 ||
				pButton->mGameMode == PAUSE_MAINMENU)
			{
				deleteObjects.push_back(pGameObjectManager->mGameObjects[i]);
			}
		}
	}
}