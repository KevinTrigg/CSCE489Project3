#include <stdio.h>
#include "Memory.h"
#include <string>



int main(){

Memory *mem = new Memory();



char fileName1[] = "Mike Smith";
char* pname1 = &fileName1[0];
char fileContent1[] = "Project Overview:\nThis project will be a an investigation into an application area of interest to you. The application area may be one that you see as a potential for your future thesis research or is of personal interest. I identified a few 'Types' of project for the purpose of managing scope and expectations for the project deliverables and grading. The project types are described in detail within the sections below. In general, I will expect a basic technical report to accompany any software, scripts, documentation, or other artifacts that are produced in the effort. These artifacts along with their relation to Operating Systems Concepts and your findings will also be presented as the final deliverable for the project.\n\nProject Types:\n\nPaper Focused\nMinimal code / lab setup\nEmphasis on literature review and synthesis of published ideas or research to reveal the latest developments or trends with respect to a given topic related to the application area as seen through the lens of OSC.\nSoftware Development Focused\nMinimal intro / background writing\nEmphasis on implementing and comparing a set of approaches / paradigms from OSC in a software project.\nOS Setup / Use Case Investigation Focused\nFor this type of project, emphasis will be on identifying and describing concepts (OSC) as they are represented/instantiated in an OS of interest or other special purpose lab setup. The concepts should be identified/described in relation to the available software (OS, or other technical elements) used in the project. The concepts of interest should also be used to structure a discussion on how the OS was modified or configured through a setup/configuration process to address a particular use case.\nTechnical Report Requirement:\n\nThe technical report should include the following elements:\n\nIntro/Conclusion - (1/2 to 1 page) includes a brief overview of the system, to include general kernel design (monolithic, microkernel, hybrid, etc)\nProcess management, thread design, and scheduler design, synchronization methods, tools, deadlock scheme, etc (1 page)\nMemory management, to include virtual memory management (i.e. how does it load programs, how does it decide what memory gets paged out, etc) (1 page)\nFile system structure/management, including directory structure, file allocation, fragmentation, etc (1 page)\nSystem security (broad overview, 1/2 to 1 page)\nThis will require some research. You are free to use the material in your textbook but do not limit yourself to that source. You do not need to cite your book but external sources should be cited appropriately. Sources can include technical journals, magazine articles, blogs or tutorials from credentialed experts (be sure to check), Wikipedia (yes, this is ok for this particular paper), etc. Do not use answers posted to questions in informal forums as you have no assurances of their accuracy.";
char* content1F1 = &fileContent1[0];
char str[] = "\n\nPapers will be graded on technical depth and accuracy. Wordy papers will be penalized--do not use more words when fewer will do. Padding or fluff is usually obvious and demonstrates a lack of effort. People write very thick books on this topic so you should be able to find enough meat where padding won't be necessary. \n\nThe paper should be written in Times New Roman 12 pt font with 1 inch margins. Major sections should have headers (bold, 14 pt font)  with double-spaces before and after the header. A Works Cited section should be at the end of your paper (does not count in page count). Use APA format for citations.\n\nThe rubric attached to this project is meant as a general guide but is not strict. I am looking for breadth and depth--a paper that covers a wide range of topics may receive max points. Similarly, a paper that covers a few topics but deep-dives into the technical weeds for each one may also receive max points. ";
char* content2F1 = &str[0];
//char str2[] = "Dragic";
//char* pteam = &str2[0];
char fileName2[] = "JOESHMOW\n TEST \t\t\t TEST";
char* pname2 = &fileName2[0];
char fileName3[] = "Mike Smithhhhhhhhhhhhhhhhhhhhh";
char* pname3 = &fileName3[0];


mem->createFile(pname3);
mem->createFile(pname1);
mem->write(pname1,content1F1);
mem->list(pname1);
mem->write(pname1,content2F1);
mem->list(pname1);
mem->createFile(pname2);
mem->dir();
//mem->list(pname1);

}
