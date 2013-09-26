//
//  TimeEntryListViewController.m
//  kopsik_ui_osx
//
//  Created by Tanel Lebedev on 19/09/2013.
//  Copyright (c) 2013 Alari. All rights reserved.
//

#import "TimeEntryListViewController.h"
#import "TimeEntryViewItem.h"
#import "UIEvents.h"
#import "kopsik_api.h"
#import "Context.h"
#import "TableViewCell.h"

@interface TimeEntryListViewController ()

@end

@implementation TimeEntryListViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
      viewitems = [NSMutableArray array];

      [[NSNotificationCenter defaultCenter]
       addObserver:self
       selector:@selector(eventHandler:)
       name:kUIEventUserLoggedIn
       object:nil];
    }
    return self;
}

-(void)eventHandler: (NSNotification *) notification
{
  if ([notification.name isEqualToString:kUIEventUserLoggedIn]) {
    char err[KOPSIK_ERR_LEN];
    TogglTimeEntryViewItemList *list = kopsik_time_entry_view_item_list_init();
    if (KOPSIK_API_SUCCESS != kopsik_time_entry_view_items(ctx, err, KOPSIK_ERR_LEN, list)) {
      NSLog(@"Error fetching time entries: %s", err);
    } else {
      [viewitems removeAllObjects];
      for (int i = 0; i < list->Length; i++) {
        TogglTimeEntryViewItem *item = list->ViewItems[i];
        TimeEntryViewItem *model = [[TimeEntryViewItem alloc] init];
        model.description = [NSString stringWithUTF8String:item->Description];
        if (item->Project) {
          model.project = [NSString stringWithUTF8String:item->Project];
        }
        if (item->Color) {
          model.color = [NSString stringWithUTF8String:item->Color];
        }
        model.duration = [NSString stringWithUTF8String:item->Duration];
        [viewitems addObject:model];
      }
    }
    kopsik_time_entry_view_item_list_clear(list);
    [self.timeEntriesTableView reloadData];
  }
}

- (int)numberOfRowsInTableView:(NSTableView *)tv
{
  return (int)[viewitems count];
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
    TimeEntryViewItem *item = [viewitems objectAtIndex:row];
    TableViewCell *cellView = [tableView makeViewWithIdentifier:@"TimeEntryCell" owner:self];
    if (cellView == nil) {
      cellView = [[TableViewCell alloc] init];
      cellView.identifier = @"TimeEntryCell";
    }
    cellView.colorTextField.backgroundColor = [self hexCodeToNSColor:item.color];
    cellView.descriptionTextField.stringValue = item.description;
    cellView.projectTextField.stringValue = item.project;
    cellView.durationTextField.stringValue = item.duration;
    return cellView;
}

- (NSColor *)hexCodeToNSColor:(NSString *)hexCode {
	unsigned int colorCode = 0;
  if (hexCode.length > 1) {
    NSString *numbers = [hexCode substringWithRange:NSMakeRange(1, [hexCode length] - 1)];
		NSScanner *scanner = [NSScanner scannerWithString:numbers];
		[scanner scanHexInt:&colorCode];
	}
	return [NSColor
          colorWithDeviceRed:((colorCode>>16)&0xFF)/255.0
          green:((colorCode>>8)&0xFF)/255.0
          blue:((colorCode)&0xFF)/255.0 alpha:1.0];
}

@end