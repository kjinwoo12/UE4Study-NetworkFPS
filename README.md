# Environment
- Window10
- [Unreal Engine4](https://www.unrealengine.com/)
  - version 4.27.2

# How to start
## 1. Fork and clone

## 2. Edit your BuildConfiguration.xml
If you don't do this step, your compiler will make errors or warnings. I recommend edit `BuildConfiguration.xml`.
In my case, the file is in `%appdata%/Roaming/Unreal Engine/UnrealBuildTool`.
```xml
<?xml version="1.0" encoding="utf-8" ?>
<Configuration xmlns="https://www.unrealengine.com/BuildConfiguration">
  <SourceFileWorkingSet>
    <Provider>None</Provider>
    <RepositoryPath></RepositoryPath>
    <GitPath></GitPath>
  </SourceFileWorkingSet>
</Configuration>
```
Please update above if you know better way about setting Git environment.
## 3. Check [`projects`](https://github.com/kjinwoo12/UE4Study-NetworkFPS/projects) and get your job.
Get the issue what you want in `To do`. Set Assignees yourself or upload your comment on the card. Then move your card in `In progress`. After your Pull Request is merged, move your card to `Done` and close the issue.

# Reference
- Documents about source code are in `./Documents` directory.
- We followed the [UE4 style guide](https://github.com/Allar/ue5-style-guide).