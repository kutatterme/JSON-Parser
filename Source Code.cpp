#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

char* buffer;
char FileName[50];
int i = 0,
rc;
char TagAlong[300],
TagAlong2[400];

void CopyFileToStr(char FileName[])
{
    FILE    *infile;
    long    numbytes;

    //open an existing file for reading
    infile = fopen(FileName, "r");

    //Get the number of bytes
    fseek(infile, 0L, SEEK_END);
    numbytes = ftell(infile);
     
    //reset the file position indicator to 
    //the beginning of the file 
    fseek(infile, 0L, SEEK_SET);	
     
    // grab sufficient memory for the 
    //buffer to hold the text
    buffer = (char*)calloc(numbytes, sizeof(char));	
 
    //copy all the text into the buffer
    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);
}
void ParseFile(char temp []) 
{
    const cJSON* name,
        * pass,
        * ip,
        * repos,
        * children,
        * children_1,
        * children_2,
        * children_3,
        * root,
        * root_path;

    cJSON* monitor_json = cJSON_Parse(temp);
  
    name = cJSON_GetObjectItemCaseSensitive(monitor_json, "user_name");    
    pass = cJSON_GetObjectItemCaseSensitive(monitor_json, "user_password");
    ip = cJSON_GetObjectItemCaseSensitive(monitor_json, "repo_ip");

    repos = cJSON_GetObjectItemCaseSensitive(monitor_json, "repos");
    cJSON_ArrayForEach(children, repos)
    {
        children_1 = cJSON_GetObjectItemCaseSensitive(children, "children");
        cJSON_ArrayForEach(children_2, children)
        {
            children_1 = cJSON_GetObjectItemCaseSensitive(children_2, "children");
            cJSON_ArrayForEach(root, children_2)
            {
                children_1 = cJSON_GetObjectItemCaseSensitive(root, "root_path");
                cJSON_ArrayForEach(children_3, root)
                {
                    children_1 = cJSON_GetObjectItemCaseSensitive(children_3, "root_path");
                    sprintf(TagAlong, "INTERRA/%s", children_1->valuestring);
                    children_1 = cJSON_GetObjectItemCaseSensitive(children_3, "url");
                    sprintf(TagAlong2, "git clone https://%s:%s@%s/%s %s", 
                        name->valuestring,
                        pass->valuestring,
                        ip->valuestring,
                        children_1->valuestring,
                        TagAlong);
                    puts(TagAlong2);
                }
            }
        }
    }
}
int main(int argc, char *argv[])
{
    _mkdir("INTERRA");

    if (argc == 2)
    {
        CopyFileToStr(argv[1]);
    }
    else if (argc == 1)
    {
        puts("\n\n.JSON FileName");
        if (scanf("%s", FileName) == NULL)
        {
            printf("\nFILE NOT FOUND...\n");
            return 0;
        }
        CopyFileToStr(FileName);
    }
    ParseFile(buffer);
    system("pause");
    return 0;
}
