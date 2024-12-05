# Game Programming Project Read Me

### Project 2
- **Components Are Used To Give Objects Various Forms Of Functionality**
- **Press The WASD Keys To Move Hero**
- ![Before Image](ProjectScreenshots/CharacterMoveBefore.png)
- ![After Image](ProjectScreenshots/CharacterMoveAfter.png)
- **Press The Space Bar To Have The Enemies Slide**
- ![Before Image](ProjectScreenshots/ZombieSlideBefore.png)
- ![After Image](ProjectScreenshots/ZombieSlideAfter.png)
- **The Hero Now Turns To Face Mouse Position**
- ![Before Image](ProjectScreenshots/HeroTurnBefore.png)
- ![After Image](ProjectScreenshots/HeroTurnAfter.png)
- **All Objects Are Now Loaded From An XML At The Start**
- ![Code Image](ProjectScreenshots/LevelXML.png)
- ![Program Image](ProjectScreenshots/LevelXMLLoaded.png)

### Project 2 Extra Credit 
- **Have Now Added The Capability For Objects To Dynamically Be Created And Destroyed**
- ![Function Code Image](ProjectScreenshots/AddAndDeleteFunctions.png)
- ![Update Code Image](ProjectScreenshots/AddAndDeleteUpdate.png)
- ![Program Image](ProjectScreenshots/BulletAdditionAndDeletion.png)

### Project 3
- **All Images Are Now Loaded Dynamically**
- ![Code Image](ProjectScreenshots/GetOrLoadFunction.png)
- ![Program Image](ProjectScreenshots/GetOrLoadFunctionInUse.png)
- Note: For Some reason, it kept saying the image was loaded, no matter what i did, so I simply chose to ignore it, since the code did what was necessary, and the logs were only used for testing
- **A View Class Is Now Added That Can Change The Camera View**
- ![Class Image](ProjectScreenshots/ViewClass.png)
- ![Code Image](ProjectScreenshots/ViewInEngineUpdate.png)
- ![Before Image](ProjectScreenshots/ViewProgramBefore.png)
- ![After Image](ProjectScreenshots/ViewProgramAfter.png)
- **There Is Now An Equation Used To Get Deltatime And Limit The Frame Rate For A Consistent FPS**
- ![Code Image](ProjectScreenshots/DeltaTimeFPSFunction.png)
- ![Program Image](ProjectScreenshots/DeltaTimeFPSProgram.png)

### Project 4
- **The Engine Has Now Been Integrated To Produce A Box2d World, And All Game Objects Are Integrated With Box2d Bodies**
- ![Initialize Code](ProjectScreenshots/B2worldInitialize.png)
- ![Update Code](ProjectScreenshots/B2worldStepUpdate.png)
- ![B2Body Code](ProjectScreenshots/B2bodyObjectInitialize.png)
- **Functions Have Been Updated To Now Set Linear Force And Angular Velocity**
- ![Code Image](ProjectScreenshots/B2bodyBodycomponentInitialize.png)
- ![Program Before Image](ProjectScreenshots/B2bodyMoveBefore.png)
- ![Program After Image](ProjectScreenshots/B2bodyMoveAfter.png)
- **Implemented A Contact Listener To Be Able To Detect And Respond To Collisions**
- ![Before Image](ProjectScreenshots/B2worldCollisionBefore.png)
- ![After Image](ProjectScreenshots/B2worldCollisionAfter.png)
- **Added The Ability To Dynamically Add And Remove Bodies**
- ![Before Image](ProjectScreenshots/B2worldDynamicBodyCreation.png)
- ![After Image](ProjectScreenshots/B2worldDynamicBodyDeletion.png)
- ![Boddy Deletion Code Image](ProjectScreenshots/DynamicB2bodyDeletionCode.png)

## Project 5
- **Added AI Behavior To Zombies**
- ![Zombie State Code](ProjectScreenshots/HeroComponentProximityCheck.png)
- **When Greater Than 400 Pixels Away From The Hero, Or If The Hero Does Not Exist, They Will Slide From Side To Side**
- ![Before Pursuit Code](ProjectScreenshots/UnawareStateCode.png)
- ![Before Pursuit Image](ProjectScreenshots/BeforeZombiePursuit.png)
- **When The Hero Exists, And Is Within 400 Pixels, The Zombie Will Continuously Pursue The Hero**
- ![Pursuit Code](ProjectScreenshots/AwareStateCode.png)
- ![Pursuit Image](ProjectScreenshots/AfterZombiePursuit.png)
- **The Contact Listener And The Damage Function For The Zombie Ensures That When Continually Contacting The Hero, The Zombie Will Damage The Hero Once Per Second**
- ![Contact Listener Code](ContactListenerPostsolve.png)
- ![Zombie Damage Code](ProjectScreenshots/zombieDamageWaiting.png)