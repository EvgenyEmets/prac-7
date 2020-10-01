#include <WorkMod.h>
#include <iostream>

using namespace com::sun::star::awt;
using namespace com::sun::star::frame;
using namespace com::sun::star::system;
using namespace com::sun::star::uno;
using namespace com::sun::star::text;
using namespace com::sun::star::table;
using namespace com::sun::star::lang;
using namespace com::sun::star::beans;
using namespace com::sun::star::sheet;
using namespace com::sun::star::util;
using namespace ::rtl;
using namespace com::sun::star::container;

void NewFile() {
    Reference< XComponentContext > xContext( ::cppu::defaultBootstrap_InitialComponentContext() );
    Reference< XMultiComponentFactory > xServiceManager = xContext->getServiceManager();
    Reference< XInterface > xDesktop = xServiceManager->createInstanceWithContext( OUString("com.sun.star.frame.Desktop"), xContext );
    Reference< XDesktop2 > xDesktop2( xDesktop, UNO_QUERY );
    Reference< XComponent > xComponent = xDesktop2->loadComponentFromURL( OUString( "private:factory/swriter" ), OUString( "_blank" ), 0, Sequence < ::com::sun::star::beans::PropertyValue >() );
    Reference< XTextDocument > xTextDocument(xComponent, UNO_QUERY);

    Reference< XText > xText = xTextDocument->getText();

    std::srand(std::time(nullptr));
    int n = std::rand() % 7 + 2;
    for (int tmp = 0; tmp < n; tmp++){
        Reference< XText > xText = xTextDocument->getText();
        Reference< XTextCursor> xTextCursor = xText->createTextCursor();
        xTextCursor->gotoEnd(false);

        {
            Reference<XTextRange> xTextRange = xText->getEnd();
            std::string s = "Table" + std::to_string(tmp + 1);
            xTextRange->setString(OUString::createFromAscii(s.c_str()));
        }

        Reference<XMultiServiceFactory> oDocMSF (xTextDocument,UNO_QUERY);
        Reference <XTextTable> xTable (oDocMSF->createInstance(
	            OUString::createFromAscii("com.sun.star.text.TextTable")),UNO_QUERY);
        if (!xTable.is()) {
            printf("Erreur creation XTextTable interface !\n");
            return;
        }
        int row = std::rand() % 8 + 3;
        int col = std::rand() % 4 + 3;
        xTable->initialize(row, col);


        Reference<XTextRange> xTextRange = xText->getEnd();
        xTextRange = xText->getEnd();
        Reference <XTextContent>xTextContent (xTable,UNO_QUERY);
        xText->insertTextContent(xTextRange, xTextContent,(unsigned char) 0);
        
        for (int tmp1 = 0; tmp1 < row; tmp1++) {
            char R = 'A';
            for (int tmp2 = 0; tmp2 < col; tmp2++) {
                std::string s = std::string({R}) + std::to_string(tmp1 + 1);
                Reference<XCell> xCell = xTable->getCellByName(OUString::createFromAscii(s.c_str()));
                xText = Reference<XText>(xCell,UNO_QUERY);
                xTextCursor = xText->createTextCursor();
                std::string s1 = "R" + std::to_string(tmp1 + 1) + " C" + std::to_string(tmp2 + 1);
                xTextCursor->setString(OUString::createFromAscii(s1.c_str()));
                R++;
            }
        }
        xTextCursor = xText->createTextCursor();
        xTextRange = xText->getEnd();
    }
}

void ItsATrans () {
    Reference< XComponentContext > xContext( ::cppu::defaultBootstrap_InitialComponentContext() );
    Reference< XMultiComponentFactory > xServiceManager = xContext->getServiceManager();
    Reference< XInterface > xDesktop = xServiceManager->createInstanceWithContext( OUString("com.sun.star.frame.Desktop"), xContext );
    Reference< XDesktop2 > xDesktop2( xDesktop, UNO_QUERY );
    Reference< XComponent > xComponent = xDesktop2->getCurrentComponent();
    Reference< XTextDocument > xTextDocument(xComponent, UNO_QUERY);
    //Reference< XText > xText = xTextDocument->getText();
    Reference< XTextTablesSupplier > xTablesSupplier(xTextDocument, UNO_QUERY);
    Reference< XNameAccess > Tables = xTablesSupplier->getTextTables();
    Sequence <rtl::OUString> TableNames = Tables->getElementNames();
    for (auto &TableName: TableNames) {
        Reference< XTextTable > xTable (Tables->getByName(TableName), UNO_QUERY);
        int row = xTable->getRows()->getCount();
        int col = xTable->getColumns()->getCount();
        int sz = row < col ? row : col;
        for (int i = 0; i < sz; i ++) {
            for (int j = i + 1; j < sz; j++) {
                std::string target = std::string({'A' + i}) + std::to_string(j + 1);
                Reference< XCell > first_cell = xTable->getCellByName(OUString::createFromAscii(target.c_str()));
                Reference< XText > first_text = Reference<XText>(first_cell, UNO_QUERY);
                 
                std::string inv_target = std::string({'A' + j}) + std::to_string(i + 1);
                Reference< XCell > second_cell = xTable->getCellByName(OUString::createFromAscii(inv_target.c_str()));
                Reference< XText > second_text = Reference<XText>(second_cell, UNO_QUERY);
                auto bubble = first_text->getString();
                first_text->setString(second_text->getString());
                second_text->setString(bubble);
            }
        }
    }
}
