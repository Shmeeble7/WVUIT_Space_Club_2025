# WVUIT_Space_Club_2025
This is the source code for the various project being worked on in 2025/26 by the WVUIT Space Club

# How to use this repository?
To properly use this repository you need to clone it onto your device. I normally do this using the git cli but it can be done with other gui tools like
github desktop. The steps to do it with the cli are the following
1) Click on the big green code button on the home page
2) Copy the https link to the repository
3) Open the git cli and type git clone "paste the https link here"
       Note: Pasting sometimes includes characters like [[ or ~ at the end and beginning of the link so be sure to delete them
   
This should create a local version of this repository onto your device. From there you can open the files in your IDE, make your changes, and then push the changes to this remote branch. The steps to push the changes using the CLI is included below. 
1) Open the cli and navigate to the location of your local copy of the repository. This can be achieved by using ls to list files in a directory and the cd to move to it
2) Stage your local changes using git stage -A. To view pending changes use git status
       Note: -A is a switch that basically means stage all. If you want to stage files individually just type the name of the changed file instead of -A.
3) commit your changes with a short text description of what they do using git commit -m "I removed 13 load bearing files :)"
4) push your local changes to the remote repository using git push -u origin "branch"
       Note: origin means push to the original repository you coppied from and the branch string should match the branch you're pushig to. For this project it will be                 something like Orbit, Scribe, or Pathfinder but NOT main. main is a protected branch and cannot be pushed to. I will handle merging things into main
   
# Questions
Please contact me or Ryan at our school emails if you have any questions or concerns regarding your projects
