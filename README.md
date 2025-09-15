# MeshNetClient
This repo probably won't have much documentation on it because all of it's in the server repo (<a href="https://github.com/80HD-0/MeshNetServer">here</a>)
<a href="https://trello.com/invite/b/68c7a784d03d36e302e0d9dd/ATTIa1839df58c4c156fbfe0cbfc5ed693e9DC881950/meshnetclient">Trello todo board</a>

# Building it
Use VSCode or the releases (you'll need to create the /build/Debug directories if you want to do VSCode)

## Versioning scheme
Example: 1   .   2   .   3<br><br>
first number (1): version. This dictates the major client release. Should a major architecture change happen, this will update.<br><br>
second number (2): release. This is always consistant with protocol version. A 0.2.x server must bind to a 0.2.x client, though some later backwards compatibility is planned.<br><br>
third number (3): subrelease. This updates when bugfixes or general client improvements happen, and updates seperately from the server.
