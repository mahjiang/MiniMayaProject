#include "deformer.h"

using namespace std;

Deformer::Deformer()
{

}

Deformer::Deformer(string filename)
{
    this->filename = filename.c_str();
    createMesh();
}

void Deformer::createMesh() {
    mesh = new Mesh();

    ifstream stream(filename);
    char line[500];
    int index = 1;
    string newline;
    if (stream.is_open()) {
        while (!stream.eof()) {
            stream.getline(line, 500);

            // removing starting whitespace and find keyword
            string keyword = "";
            for (int i = 0; i < 500; i++) {
                if (line[i] == ' ' || line[i] == '\t')
                    continue;
                else {
                    while (line[i] != ' ' && line[i] != '\t') {
                        keyword+=line[i];
                        i++;
                    }
                    newline.assign(line);
                    newline = newline.substr(i+1, 500);
                    break;
                }
            }

            //vertex
            if (!keyword.compare("v")) {
                float x, y, z;
                sscanf(newline.c_str(),"%f %f %f", &x, &y, &z);
                vertices[index] = new Vertex(mesh->getVid(), x, y, z);
                index++;
            }

            //face
            else if (!keyword.compare("f")) {
                // get vertices
                vector<int> indices;
                int v1, v2, v3, v4, over;
                stringstream ss(newline);
                ss>>v1;
                if (v1 > 0)
                    indices.push_back(v1);
                else
                    indices.push_back(vertices.size() + v1);
                ss>>v2;
                if (v2 > 0)
                    indices.push_back(v2);
                else
                    indices.push_back(vertices.size() + v2);
                ss>>v3;
                if (v3 > 0)
                    indices.push_back(v3);
                else
                    indices.push_back(vertices.size() + v3);
                if (ss>>v4) {
                    if (v4 > 0)
                        indices.push_back(v4);
                    else
                        indices.push_back(vertices.size() + v4);
                }
                if (ss>>over) {
                    QErrorMessage errorMessage;
                    errorMessage.showMessage("Please ensure that polygons have four or fewer vertices");
                    errorMessage.exec();
                    mesh = NULL;
                    break;
                }

                // make face and half edges
                Face *curr = new Face(mesh->getFid());
                HalfEdge *last = NULL;
                for (unsigned int i = 0; i < indices.size(); i++) {
                    HalfEdge *he = new HalfEdge(mesh->getHEid());
                    he->vert = vertices[indices[i]];
                    he->face = curr;
                    if (last != NULL)
                        last->next = he;
                    else
                        curr->halfedge = he;
                    last = he;
                    if (i == (indices.size()-1))
                        last->next = curr->getHE();
                }
                mesh->addFace(curr);

                // assign symmetric half-edges
                vector<Face*>* faces = mesh->faceList;
                if (faces->size() > 0) {
                    HalfEdge *traverse = curr->getHE();
                    do {
                        Vertex *v1 = traverse->vert;
                        Vertex *v2 = traverse->next->vert;
                        for (unsigned int i = 0; i < faces->size(); i++) {
                            HalfEdge *traverse2 = mesh->faceAt(i)->getHE();
                            do {
                                if (traverse2->vert == v2 && traverse2->next->vert == v1) {
                                    traverse->sym = traverse2;
                                    traverse2->sym = traverse;
                                }
                                traverse2 = traverse2->next;
                            } while (traverse2 != mesh->faceAt(i)->getHE());
                        }
                        traverse = traverse->next;
                    } while (traverse != curr->getHE());
                }
            }

            else if (!keyword.compare("mtllib")) {
                ifstream mtlStream(newline.c_str());
                char mtlLine[500];
                string mtlNewline;
                string mtl = "";
                vector<vector<int> > values;

                if (mtlStream.is_open()) {
                    while (!mtlStream.eof()) {
                        mtlStream.getline(mtlLine, 500);
                        string mtlKeyword = "";

                        for (int i = 0; i < 500; i++) {
                            if (mtlLine[i] == ' ' || mtlLine[i] == '\t')
                                continue;
                            else {
                                while (mtlLine[i] != ' ' && mtlLine[i] != '\t') {
                                    mtlKeyword+=mtlLine[i];
                                    i++;
                                }
                                mtlNewline.assign(mtlLine);
                                mtlNewline = mtlNewline.substr(i+1, 500);
                                break;
                            }
                        }

                        if (!mtlKeyword.compare("newmtl")) {
                            mtl = mtlNewline;
                            values.clear();
                        }

                        if (!mtlKeyword.compare("Ka")) {
                            float r, g, b;
                            sscanf(mtlNewline.c_str(),"%f %f %f", &r, &g, &b);
                            vector<int> ambient;
                            ambient.push_back(r);
                            ambient.push_back(g);
                            ambient.push_back(b);
                        }

                        else if (!mtlKeyword.compare("Kd")) {
                            float r, g, b;
                            sscanf(mtlNewline.c_str(),"%f %f %f", &r, &g, &b);
                            vector<int> diffuse;
                            diffuse.push_back(r);
                            diffuse.push_back(g);
                            diffuse.push_back(b);
                        }
                    }
                }
                mtlStream.close();
            }
        }
        stream.close();
    }
}

Mesh* Deformer::getMesh() {
    return mesh;
}

void Deformer::exportMesh(string fn, Mesh *m)
{
    this->filename = fn.c_str();
    ofstream stream(filename);
    set<Vertex*>* verticesTemp = m->getVertexSet();
    set<Vertex*> vertices = *verticesTemp;
    set<Vertex*>::iterator it;
    if (stream.is_open()) {
        // write vertices
        for (it = vertices.begin(); it != vertices.end(); ++it) {
            Vertex* v = *it;
        }

        vector<Face*>* facesTemp = m->faceList;
        vector<Face*> faces = *facesTemp;
        for (unsigned int i = 0; i < faces.size(); i++) {
            HalfEdge* traverse = faces[i]->getHE();
            // vector storing this face's vertices
            vector<Vertex*> verts;
            // vector storing the indices associated with those vertices
            vector<int> indices;
            // add all this face's vertices to verts
            do {
                verts.push_back(traverse->vert);
                traverse = traverse->next;
            } while (traverse != faces[i]->getHE());

            // for each of the face's vertices, which which index it corresponds to in the original vertex set
            int index = 1;
            for (unsigned int j = 0; j < verts.size(); j++) {
                Vertex *v = verts[j];
                for (it = vertices.begin(); it != vertices.end(); ++it) {
                    Vertex *vtemp = *it;
                    if (v == vtemp) {
                        indices.push_back(index);
                        index = 1;
                        break;
                    }
                    index++;
                }
            }
            // write faces
            stream<<"f ";
            for (unsigned int j = 0; j < indices.size(); j++) {
                stream<<indices[j]<<" ";
                if (j == indices.size()-1 && i != faces.size()-1)
                    stream<<"\n";
            }
        }
        stream.close();
    }
}
