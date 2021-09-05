/* eslint-disable max-len */
import { ComponentFixture, TestBed } from '@angular/core/testing';
import { ReactiveFormsModule } from '@angular/forms';

import { TranslateMockPipe } from 'toolbox';

import { FoundationStripFormComponent } from './foundation-strip-form.component';
/* eslint-enable max-len */

describe('FoundationStripFormComponent', () => {
  let component: FoundationStripFormComponent;
  let fixture: ComponentFixture<FoundationStripFormComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ReactiveFormsModule],
      declarations: [FoundationStripFormComponent, TranslateMockPipe]
    }).compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(FoundationStripFormComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
