
# The world of blacktyger

Unreal engine c++ Game  

C++ code from one of my first games published on itch.io:
[Link Itch.io para descargar](https://vicagent.itch.io/the-world-of-black-tyger)

With this free Epic code and art, you can quickly have a third-person shooter with the following mechanics: 
1.  Weapon swapping between shooting and melee weapons.
2.  Simple AI using a 3-state pawnsensing component (Idle, Suspicious, Alerted).
3.  Floating item management that can be used in Blueprints to make enemies drop "something" of value (crowns).
4.  Health and armor management. 5. Item management (AMMO, crowns).

This is a simple project for learning in a fun way, showcasing many interesting concepts related to Unreal Engine development:

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)


## installation
1. Clone the repository:
```bash
 git clone https://github.com/jorgemmm/TheWorfdofBlackTygerCodeOnly.git
```
or download zip.
2.requeriments
you need unreal engine and visual studio.


You need create a third person shooter and 
Create the same C++ actors as the project you just downloaded (with their attributes and methods), 
(use your own names if you want but keep in mind that some attribute may refer to that new class name)

You must add to skeletalmesh character a socket name in the hand (for example) at the righ location and righ rotation,  in order to spawn and attach the weapon correctly to the character.
```bash
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseCharacter")
FName WeaponSocketName
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseCharacter")
FName GunSocketName
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseCharacter")
FName UnequipWeaponSocketName;
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseCharacter")
FName UnequipGunSocketName;
```

Idem to weapon socket name, but in order to spawn the Fx fire correctly
```bash
UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
FName MuzzleSocketName;
```

3.know issues
for 4.26+ UCameraShake require this change:
```bash
//in  yourweapon.h file
//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
//TSubclassOf<class UCameraShake> FireCamShake;
TSubclassOf<class UCameraShakeBase> FireCamShake;

//in  yourweapon.cpp file
//#include "Camera/CameraShake.h"
#include "Camera/CameraShakeBase.h"

```
It doesn't matter if you remove this component for another type od feedback

You must create a an actor Close Combat (CC) from Blueprint
and Weapon Combat distance actor from c++ weapon class,
in order to add to base character and equip/swap weapon mechanics work correctly 

```bash
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter")
AActor* AxeWeapon;
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
TSubclassOf<class AActor> AxeWeaponClass;		
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter")		
class ABTWeapon* GunWeapon;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
TSubclassOf<class ABTWeapon> GunWeaponClass;
```

## usage
You can use this project with paragon or infinity blade free content from epic
Characters: [Paragon Characters](https://www.fab.com/search?is_free=1&listing_types=3d-model&q=paragon&categories=characters-creatures)
Eviroment: [Agora from Paragon](https://www.fab.com/listings/6f401fb5-88b5-41b4-bf1b-62321414e1f0)
Enemies, Minions and Bosses, CC weapons: [Minions from Paragon](https://www.fab.com/listings/039ea035-9360-4e76-ad06-5d3a92da6f65)


  
## contributing
1. Fork the repository.
2. Create a new branch: `git checkout -b feature-name`.
3. Make your changes.
4. Push your branch: `git push origin feature-name`.
5. Create a pull request.

# license
This project is licensed under the [MIT License](https://mit-license.org/).
