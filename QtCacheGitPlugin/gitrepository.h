/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GITREPOSITORY_H
#define GITREPOSITORY_H

#include <QString>
#include <QDir>

struct git_repository;
struct git_signature;

class GitRepository
{
public:
    GitRepository(const QString& localDirPath);
    ~GitRepository();

    bool isOpen();

    void open();
    void init();

    void branch(const QString& name);
    void add(const QString& filepath);
    void commit(const QString& message);

    void clone(const QString& url);
    void push();
    void fetch();

    void setSignature(const QString& authorName = "LibQtCache GitPlugin", const QString& authorEmail = "entwickler42@gmail.com");

private:
    QDir m_local_dir_path;
    git_repository* m_repo;
    git_signature* m_signature;

    void setRepository(git_repository* repo);
    git_repository* repository();
    git_signature* signature();
};

#endif // GITREPOSITORY_H
