import { ComponentFixture, TestBed } from '@angular/core/testing';

import { MeyerhofFormComponent } from './meyerhof-form.component';

describe('MeyerhofFormComponent', () => {
  let component: MeyerhofFormComponent;
  let fixture: ComponentFixture<MeyerhofFormComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [MeyerhofFormComponent]
    }).compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(MeyerhofFormComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
