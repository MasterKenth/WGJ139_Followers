# (WGJ#139) Cult Wars
Defeat enemy cult followers and force them to join your own cult! May the biggest cult win.

My entry for the Weekly Game Jam #139 (theme: followers).

## Summary
Cult Wars is a 2D top-down round-based arena fighter with a battle-royale-like element. You play as a cult leader and 
"defeat" enemy cult followers in order to convert them to your own cult. Each round is time-based and after each round 
you tally up the cult followers and begin the next round with your new followers.

The goal is to be the last cult standing with all the followers following your every command.

## Controls
| Input     | Action       |
| --------- | ------------ |
| WASD      | Move         |
| Space/LMB | Attack       |
| M         | Toggle music |
| ESC       | Quit game    |

## Game Jam Post Mortem
This project was part of the Weekly Game Jam for week #139 where the theme was _Followers_. I decided to use this project to improve my skill with Unreal Engine 4, particularly the 2D aspect of it. I also wanted to run solo since I want to use game jams to specifically improve my own abilities in every aspect of game development and raise the bar for myself.

Every asset (except for fonts and some built-in UI elements) was made by myself during the jam duration. This includes code, sprites, and audio.

The following things are aspects of the engine that I had very little experience with or had never touched until now:
* Paper2D flipbooks
* Collision in 2D space
* Behavior Trees and AI Tasks

Throughout the project I also took the opportunity to experiment with using both [Semantic Commit Messages](https://seesparkbox.com/foundry/semantic_commit_messages) and the [Gamemakin UE4 Style Guide by Allar](https://github.com/Allar/ue4-style-guide) which worked fairly well and helped keep things organized.

### Improvements
As with any game jam time is of the essence, and as such a few features had to be cut that I would've liked to include but had no time to.

* Sound FX
  * I didn't want to use premade sfx and so there are none as I did not have time to create and properly attenuate things like hit and death sounds.
* Feedback effects (hit/death particle systems, screen shake etc.)
  * This would've helped a lot to set it apart as a more polished slice. Right now, the gameplay feel is a bit lacking.
* Pathfinding/Collision avoidance
  * Right now, all AI followers can easily clump which makes things a bit too easy. They also just follow their target straight without any prediction or ambushing.
* Abilities/bonuses
  * A system for gaining perks as the rounds progress would've made things a bit more interesting. Things like movement buffs, new attacks etc.
* Animations (idle, run, etc.)
* Minimap (so you don't get lost and can focus your movement better)
* Improved UI (always see current followers etc.)
* Main Menu
